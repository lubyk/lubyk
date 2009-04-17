/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "_cxcore.h"

namespace cv
{

#if 1

#if 0
#define SANITY_CHECK(block) \
    CV_Assert(((size_t)(block) & (MEM_BLOCK_SIZE-1)) == 0 && \
        (unsigned)(block)->binIdx <= (unsigned)MAX_BIN && \
        (block)->signature == MEM_BLOCK_SIGNATURE)
#else
#define SANITY_CHECK(block)
#endif

#define STAT(stmt)

#ifdef WIN32
struct CriticalSection
{
    CriticalSection() { InitializeCriticalSection(&cs); }
    ~CriticalSection() { DeleteCriticalSection(&cs); }
    void lock() { EnterCriticalSection(&cs); }
    void unlock() { LeaveCriticalSection(&cs); }
    bool trylock() { return TryEnterCriticalSection(&cs) != 0; }

    CRITICAL_SECTION cs;
};

void* SystemAlloc(size_t size)
{
    void* ptr = VirtualAlloc(0, size, MEM_RESERVE + MEM_COMMIT, PAGE_READWRITE);
    return ptr;
}

void SystemFree(void* ptr, size_t)
{
    VirtualFree(ptr, 0, MEM_RELEASE);
}
#else
struct CriticalSection
{
    CriticalSection() { pthread_mutex_init(&mutex, 0); }
    ~CriticalSection() { pthread_mutex_destroy(&mutex); }
    void lock() { pthread_mutex_lock(&mutex); }
    void unlock() { pthread_mutex_unlock(&mutex); }
    bool trylock() { return pthread_mutex_trylock(&mutex) == 0; }

    pthread_mutex_t mutex;
};

void* SystemAlloc(size_t size)
{
    #ifndef MAP_ANONYMOUS
    #define MAP_ANONYMOUS MAP_ANON
    #endif
    void* ptr = 0;
    ptr = mmap(ptr, size, (PROT_READ | PROT_WRITE), MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    return ptr != MAP_FAILED ? ptr : 0;
}

void SystemFree(void* ptr, size_t size)
{
    munmap(ptr, size);
}
#endif

struct AutoLock
{
    AutoLock(CriticalSection& _cs) : cs(&_cs) { cs->lock(); }
    ~AutoLock() { cs->unlock(); }
    CriticalSection* cs;
};

const size_t MEM_BLOCK_SIGNATURE = 0x01234567;
const int MEM_BLOCK_SHIFT = 14;
const size_t MEM_BLOCK_SIZE = 1 << MEM_BLOCK_SHIFT;
const size_t HDR_SIZE = 128;
const size_t MAX_BLOCK_SIZE = MEM_BLOCK_SIZE - HDR_SIZE;
const int MAX_BIN = 28;

static const int binSizeTab[MAX_BIN+1] =
{ 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 128, 160, 192, 256, 320, 384, 480, 544, 672, 768,
896, 1056, 1328, 1600, 2688, 4048, 5408, 8128, 16256 };

struct MallocTables
{
    void initBinTab()
    {
        int i, j = 0, n;
        for( i = 0; i <= MAX_BIN; i++ )
        {
            n = binSizeTab[i]>>3;
            for( ; j <= n; j++ )
                binIdx[j] = (uchar)i;
        }
    }
    int bin(size_t size)
    {
        assert( size <= MAX_BLOCK_SIZE );
        return binIdx[(size + 7)>>3];
    }

    MallocTables()
    {
        initBinTab();
    }

    uchar binIdx[MAX_BLOCK_SIZE/8+1];
};

MallocTables mallocTables;

struct Node
{
    Node* next;
};

struct ThreadData;

struct Block
{
    Block(Block* _next)
    {
        signature = MEM_BLOCK_SIGNATURE;
        prev = 0;
        next = _next;
        privateFreeList = publicFreeList = 0;
        bumpPtr = endPtr = 0;
        objSize = 0;
        threadData = 0;
        data = (uchar*)this + HDR_SIZE;
    }

    ~Block() {}

    void init(Block* _prev, Block* _next, int _objSize, ThreadData* _threadData)
    {
        prev = _prev;
        if(prev)
            prev->next = this;
        next = _next;
        if(next)
            next->prev = this;
        objSize = _objSize;
        binIdx = mallocTables.bin(objSize);
        threadData = _threadData;
        privateFreeList = publicFreeList = 0;
        bumpPtr = data;
        int nobjects = MAX_BLOCK_SIZE/objSize;
        endPtr = bumpPtr + nobjects*objSize;
        almostEmptyThreshold = (nobjects*2 + 1)/3;
        allocated = 0;
    }

    size_t signature;
    Block* prev;
    Block* next;
    Node* privateFreeList;
    Node* publicFreeList;
    uchar* bumpPtr;
    uchar* endPtr;
    uchar* data;
    ThreadData* threadData;
    int objSize;
    int binIdx;
    int allocated;
    int almostEmptyThreshold;
    int isFilled;
    CriticalSection cs;
};

struct BigBlock
{
    BigBlock(int bigBlockSize, BigBlock* _next)
    {
        first = alignPtr((Block*)(this+1), MEM_BLOCK_SIZE);
        next = _next;
        nblocks = ((char*)this + bigBlockSize - (char*)first)/MEM_BLOCK_SIZE;
        Block* p = 0;
        for( int i = nblocks-1; i >= 0; i-- )
            p = ::new((uchar*)first + i*MEM_BLOCK_SIZE) Block(p);
    }

    ~BigBlock()
    {
        for( int i = nblocks-1; i >= 0; i-- )
            ((Block*)((uchar*)first+i*MEM_BLOCK_SIZE))->~Block();
    }

    BigBlock* next;
    Block* first;
    int nblocks;
};

struct BlockPool
{
    BlockPool(int _bigBlockSize=1<<20) : pool(0), bigBlockSize(_bigBlockSize)
    {
    }

    ~BlockPool()
    {
        AutoLock lock(cs);
        while( pool )
        {
            BigBlock* nextBlock = pool->next;
            pool->~BigBlock();
            SystemFree(pool, bigBlockSize);
            pool = nextBlock;
        }
    }

    Block* alloc()
    {
        AutoLock lock(cs);
        Block* block;
        if( !freeBlocks )
        {
            BigBlock* bblock = ::new(SystemAlloc(bigBlockSize)) BigBlock(bigBlockSize, pool);
            assert( bblock != 0 );
            freeBlocks = bblock->first;
            pool = bblock;
        }
        block = freeBlocks;
        freeBlocks = freeBlocks->next;
        if( freeBlocks )
            freeBlocks->prev = 0;
        STAT(stat.bruttoBytes += MEM_BLOCK_SIZE);
        return block;
    }

    void free(Block* block)
    {
        AutoLock lock(cs);
        block->prev = 0;
        block->next = freeBlocks;
        freeBlocks = block;
        STAT(stat.bruttoBytes -= MEM_BLOCK_SIZE);
    }

    CriticalSection cs;
    Block* freeBlocks;
    BigBlock* pool;
    int bigBlockSize;
    int blocksPerBigBlock;
};

BlockPool mallocPool;

struct ThreadData
{
    ThreadData() { for(int i = 0; i <= MAX_BIN; i++) bins[i] = 0; }
    ~ThreadData()
    {
        // mark all the thread blocks as abandoned or even release them
        for(int i = 0; i <= MAX_BIN; i++)
        {
            Block *bin = bins[i], *block = bin;
            bins[i] = 0;
            if( block )
            {
                do
                {
                    Block* next = block->next;
                    int allocated = block->allocated;
                    {
                    AutoLock lock(block->cs);
                    block->next = block->prev = 0;
                    block->threadData = 0;
                    Node *node = block->publicFreeList;
                    for( ; node != 0; node = node->next )
                        allocated--;
                    }
                    if( allocated == 0 )
                        mallocPool.free(block);
                    block = next;
                }
                while( block != bin );
            }
        }
    }

    Block* bins[MAX_BIN+1];

#ifdef WIN32
#ifdef WINCE
#	define TLS_OUT_OF_INDEXES ((DWORD)0xFFFFFFFF)
#endif

    static DWORD tlsKey;
    static ThreadData* get()
    {
        ThreadData* data;
        if( tlsKey == TLS_OUT_OF_INDEXES )
            tlsKey = TlsAlloc();
        data = (ThreadData*)TlsGetValue(tlsKey);
        if( !data )
        {
            data = new ThreadData;
            TlsSetValue(tlsKey, data);
        }
        return data;
    }
#else
    static void deleteData(void* data)
    {
        delete (ThreadData*)data;
    }

    static pthread_key_t tlsKey;
    static ThreadData* get()
    {
        ThreadData* data;
        if( !tlsKey )
            pthread_key_create(&tlsKey, deleteData);
        data = (ThreadData*)pthread_getspecific(tlsKey);
        if( !data )
        {
            data = new ThreadData;
            pthread_setspecific(tlsKey, data);
        }
        return data;
    }
#endif
};

#ifdef WIN32
DWORD ThreadData::tlsKey = TLS_OUT_OF_INDEXES;

void deleteThreadAllocData()
{
    if( ThreadData::tlsKey != TLS_OUT_OF_INDEXES )
        delete (ThreadData*)TlsGetValue( ThreadData::tlsKey );
}

#else
pthread_key_t ThreadData::tlsKey = 0;
#endif

void* fastMalloc( size_t size )
{
    if( size > MAX_BLOCK_SIZE )
    {
        size_t size1 = size + sizeof(uchar*)*2 + MEM_BLOCK_SIZE;
        uchar* udata = (uchar*)SystemAlloc(size1);
        uchar** adata = alignPtr((uchar**)udata + 2, MEM_BLOCK_SIZE);
        adata[-1] = udata;
        adata[-2] = (uchar*)size1;
        return adata;
    }

    {
    ThreadData* tls = ThreadData::get();
    int idx = mallocTables.bin(size);
    Block *&bin = tls->bins[idx], *block = bin;
    size = binSizeTab[idx];
    STAT(
        stat.nettoBytes += size;
        stat.mallocCalls++;
        );
    uchar* data = 0;

    for(;;)
    {
        if( block )
        {
            int k = 0;
            // try to
            do
            {
                if( block->bumpPtr )
                {
                    data = block->bumpPtr;
                    if( (block->bumpPtr += size) >= block->endPtr )
                        block->bumpPtr = 0;
                    break;
                }

                if( block->privateFreeList )
                {
                    data = (uchar*)block->privateFreeList;
                    block->privateFreeList = block->privateFreeList->next;
                    break;
                }
                k++;
                block = block->next;
            }
            while( block != bin );

            if( !data )
            {
                do
                {
                    SANITY_CHECK(block);

                    if( block->publicFreeList )
                    {
                        {
                        AutoLock lock(block->cs);
                        block->privateFreeList = block->publicFreeList;
                        block->publicFreeList = 0;
                        }
                        Node* node = block->privateFreeList;
                        for(;node != 0; node = node->next)
                            --block->allocated;
                        data = (uchar*)block->privateFreeList;
                        block->privateFreeList = block->privateFreeList->next;
                        break;
                    }
                    block = block->next;
                }
                while( block != bin );
            }
        }

        if( data )
            break;
        block = mallocPool.alloc();
        block->init(bin ? bin->prev : block, bin ? bin : block, size, tls);
        SANITY_CHECK(block);
    }

    block->isFilled = ++block->allocated > block->almostEmptyThreshold;
    bin = block;
    return data;
    }
}

void fastFree( void* ptr )
{
    if( ((size_t)ptr & (MEM_BLOCK_SIZE-1)) == 0 )
    {
        if( ptr != 0 )
        {
            void* origPtr = ((void**)ptr)[-1];
            size_t sz = (size_t)((void**)ptr)[-2];
            SystemFree( origPtr, sz );
        }
        return;
    }

    {
    ThreadData* tls = ThreadData::get();
    Node* node = (Node*)ptr;
    Block* block = (Block*)((size_t)ptr & -(int)MEM_BLOCK_SIZE);
    assert( block->signature == MEM_BLOCK_SIGNATURE );

    if( block->threadData == tls )
    {
        STAT(
        stat.nettoBytes -= block->objSize;
        stat.freeCalls++;
        float ratio = (float)stat.nettoBytes/stat.bruttoBytes;
        if( stat.minUsageRatio > ratio )
            stat.minUsageRatio = ratio;
        );

        SANITY_CHECK(block);

        if( --block->allocated <= block->almostEmptyThreshold &&
            (block->allocated == 0 || block->isFilled) )
        {
            Block*& bin = tls->bins[block->binIdx];
            if( block->allocated == 0 )
            {
                if( block == block->next )
                    bin = 0;
                else
                {
                    if( bin == block )
                        bin = block->next;
                    block->prev->next = block->next;
                    block->next->prev = block->prev;
                }
                mallocPool.free(block);
                return;
            }

            block->isFilled = 0;
            if( bin != block && bin->next != block )
            {
                block->prev->next = block->next;
                block->next->prev = block->prev;

                block->prev = bin;
                block->next = bin->next;
                block->next->prev = block;
                bin->next = block;
            }
        }
        node->next = block->privateFreeList;
        block->privateFreeList = node;
    }
    else
    {
        AutoLock lock(block->cs);
        SANITY_CHECK(block);

        node->next = block->publicFreeList;
        block->publicFreeList = node;
        if( block->threadData == 0 )
        {
            // take ownership of the abandoned block.
            // note that it can happen at the same time as
            // ThreadData::deleteData() marks the blocks as abandoned,
            // so this part of the algorithm needs to be checked for data races
            block->threadData = tls;
            Block*& bin = tls->bins[block->binIdx];
            if( bin )
            {
                block->next = bin;
                block->prev = bin->prev;
                block->next->prev = block->prev->next = block;
            }
            else
                bin = block->next = block->prev = block;
        }
    }
    }
}

#else

void deleteThreadAllocData() {}

void* fastMalloc( size_t size ) { return malloc(size); }
void fastFree(void* ptr) { if(ptr) free(ptr); }

#endif

}

CV_IMPL void cvSetMemoryManager( CvAllocFunc, CvFreeFunc, void * )
{
    CV_Error( -1, "Custom memory allocator is not supported" );
}

CV_IMPL void* cvAlloc( size_t size )
{
    return cv::fastMalloc( size );
}

CV_IMPL void cvFree_( void* ptr )
{
    cv::fastFree( ptr );
}


/* End of file. */
