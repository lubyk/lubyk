/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-7 by Raw Material Software ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the
   GNU General Public License, as published by the Free Software Foundation;
   either version 2 of the License, or (at your option) any later version.

   JUCE is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with JUCE; if not, visit www.gnu.org/licenses or write to the
   Free Software Foundation, Inc., 59 Temple Place, Suite 330,
   Boston, MA 02111-1307 USA

  ------------------------------------------------------------------------------

   If you'd like to release a closed-source product which uses JUCE, commercial
   licenses are also available: visit www.rawmaterialsoftware.com/juce for
   more information.

  ==============================================================================
*/

#ifndef __JUCE_INPUTSTREAM_JUCEHEADER__
#define __JUCE_INPUTSTREAM_JUCEHEADER__

#include "../text/juce_String.h"
#include "../containers/juce_MemoryBlock.h"


//==============================================================================
/** The base class for streams that read data.

    Input and output streams are used throughout the library - subclasses can override
    some or all of the virtual functions to implement their behaviour.

    @see OutputStream, MemoryInputStream, BufferedInputStream, FileInputStream
*/
class JUCE_API  InputStream
{
public:
    /** Destructor. */
    virtual ~InputStream()  {}

    //==============================================================================
    /** Returns the total number of bytes available for reading in this stream.

        Note that this is the number of bytes available from the start of the
        stream, not from the current position.

        If the size of the stream isn't actually known, this may return -1.
    */
    virtual int64 getTotalLength() = 0;

    //==============================================================================
    /** Returns true if the stream has no more data to read. */
    virtual bool isExhausted() = 0;

    //==============================================================================
    /** Reads a set of bytes from the stream into a memory buffer.

        This is the only read method that subclasses actually need to implement, as the
        InputStream base class implements the other read methods in terms of this one (although
        it's often more efficient for subclasses to implement them directly).

        @param destBuffer       the destination buffer for the data
        @param maxBytesToRead   the maximum number of bytes to read - make sure the
                                memory block passed in is big enough to contain this
                                many bytes.

        @returns    the actual number of bytes that were read, which may be less than
                    maxBytesToRead if the stream is exhausted before it gets that far
    */
    virtual int read (void* destBuffer,
                      int maxBytesToRead) = 0;

    /** Reads a byte from the stream.

        If the stream is exhausted, this will return zero.

        @see OutputStream::writeByte
    */
    virtual char readByte();

    /** Reads a boolean from the stream.

        The bool is encoded as a single byte - 1 for true, 0 for false.

        If the stream is exhausted, this will return false.

        @see OutputStream::writeBool
    */
    virtual bool readBool();

    /** Reads two bytes from the stream as a little-endian 16-bit value.

        If the next two bytes read are byte1 and byte2, this returns
        (byte1 | (byte2 << 8)).

        If the stream is exhausted partway through reading the bytes, this will return zero.

        @see OutputStream::writeShort, readShortBigEndian
    */
    virtual short readShort();

    /** Reads two bytes from the stream as a little-endian 16-bit value.

        If the next two bytes read are byte1 and byte2, this returns
        (byte2 | (byte1 << 8)).

        If the stream is exhausted partway through reading the bytes, this will return zero.

        @see OutputStream::writeShortBigEndian, readShort
    */
    virtual short readShortBigEndian();

    /** Reads four bytes from the stream as a little-endian 32-bit value.

        If the next four bytes are byte1 to byte4, this returns
        (byte1 | (byte2 << 8) | (byte3 << 16) | (byte4 << 24)).

        If the stream is exhausted partway through reading the bytes, this will return zero.

        @see OutputStream::writeInt, readIntBigEndian
    */
    virtual int readInt();

    /** Reads four bytes from the stream as a big-endian 32-bit value.

        If the next four bytes are byte1 to byte4, this returns
        (byte4 | (byte3 << 8) | (byte2 << 16) | (byte1 << 24)).

        If the stream is exhausted partway through reading the bytes, this will return zero.

        @see OutputStream::writeIntBigEndian, readInt
    */
    virtual int readIntBigEndian();

    /** Reads eight bytes from the stream as a little-endian 64-bit value.

        If the next eight bytes are byte1 to byte8, this returns
        (byte1 | (byte2 << 8) | (byte3 << 16) | (byte4 << 24) | (byte5 << 32) | (byte6 << 40) | (byte7 << 48) | (byte8 << 56)).

        If the stream is exhausted partway through reading the bytes, this will return zero.

        @see OutputStream::writeInt64, readInt64BigEndian
    */
    virtual int64 readInt64();

    /** Reads eight bytes from the stream as a big-endian 64-bit value.

        If the next eight bytes are byte1 to byte8, this returns
        (byte8 | (byte7 << 8) | (byte6 << 16) | (byte5 << 24) | (byte4 << 32) | (byte3 << 40) | (byte2 << 48) | (byte1 << 56)).

        If the stream is exhausted partway through reading the bytes, this will return zero.

        @see OutputStream::writeInt64BigEndian, readInt64
    */
    virtual int64 readInt64BigEndian();

    /** Reads four bytes as a 32-bit floating point value.

        The raw 32-bit encoding of the float is read from the stream as a little-endian int.

        If the stream is exhausted partway through reading the bytes, this will return zero.

        @see OutputStream::writeFloat, readDouble
    */
    virtual float readFloat();

    /** Reads four bytes as a 32-bit floating point value.

        The raw 32-bit encoding of the float is read from the stream as a big-endian int.

        If the stream is exhausted partway through reading the bytes, this will return zero.

        @see OutputStream::writeFloatBigEndian, readDoubleBigEndian
    */
    virtual float readFloatBigEndian();

    /** Reads eight bytes as a 64-bit floating point value.

        The raw 64-bit encoding of the double is read from the stream as a little-endian int64.

        If the stream is exhausted partway through reading the bytes, this will return zero.

        @see OutputStream::writeDouble, readFloat
    */
    virtual double readDouble();

    /** Reads eight bytes as a 64-bit floating point value.

        The raw 64-bit encoding of the double is read from the stream as a big-endian int64.

        If the stream is exhausted partway through reading the bytes, this will return zero.

        @see OutputStream::writeDoubleBigEndian, readFloatBigEndian
    */
    virtual double readDoubleBigEndian();

    /** Reads an encoded 32-bit number from the stream using a space-saving compressed format.

        For small values, this is more space-efficient than using readInt() and OutputStream::writeInt()

        The format used is: number of significant bytes + up to 4 bytes in little-endian order.

        @see OutputStream::writeCompressedInt()
    */
    virtual int readCompressedInt();

    //==============================================================================
    /** Reads a string from the stream, up to the next linefeed or carriage return.

        The stream is treated as 8-bit characters encoded with the system's default encoding,
        and this will read up to the next "\n" or "\r\n" or end-of-stream.

        After this call, the stream's position will be left pointing to the character
        following the line-feed, but the linefeeds aren't included in the string that
        is returned.
    */
    virtual const String readNextLine();

    /** Reads a zero-terminated string from the stream.

        This will read characters from the stream until it hits a zero character or
        end-of-stream.

        @see OutputStream::writeString, readEntireStreamAsString
    */
    virtual const String readString();

    /** Tries to read the whole stream and turn it into a string.

        This will read from the stream's current position until the end-of-stream, and
        will try to make an educated guess about whether it's unicode or an 8-bit encoding.
    */
    virtual const String readEntireStreamAsString();

    /** Reads from the stream and appends the data to a MemoryBlock.

        @param destBlock            the block to append the data onto
        @param maxNumBytesToRead    if this is a positive value, it sets a limit to the number
                                    of bytes that will be read - if it's negative, data
                                    will be read until the stream is exhausted.
        @returns the number of bytes that were added to the memory block
    */
    virtual int readIntoMemoryBlock (MemoryBlock& destBlock,
                                     int maxNumBytesToRead = -1);

    //==============================================================================
    /** Returns the offset of the next byte that will be read from the stream.

        @see setPosition
    */
    virtual int64 getPosition() = 0;

    /** Tries to move the current read position of the stream.

        The position is an absolute number of bytes from the stream's start.

        Some streams might not be able to do this, in which case they should do
        nothing and return false. Others might be able to manage it by resetting
        themselves and skipping to the correct position, although this is
        obviously a bit slow.

        @returns  true if the stream manages to reposition itself correctly
        @see getPosition
    */
    virtual bool setPosition (int64 newPosition) = 0;

    /** Reads and discards a number of bytes from the stream.

        Some input streams might implement this efficiently, but the base
        class will just keep reading data until the requisite number of bytes
        have been done.
    */
    virtual void skipNextBytes (int64 numBytesToSkip);


    //==============================================================================
    juce_UseDebuggingNewOperator

protected:
    //==============================================================================
    InputStream() throw()  {}
};

#endif   // __JUCE_INPUTSTREAM_JUCEHEADER__
