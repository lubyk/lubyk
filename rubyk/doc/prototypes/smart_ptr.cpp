/** 

compile and test with:

g++ smart_ptr.cpp -o test && ./test

*/

#include <cstdio>
#include "smart_ptr.h"

class Data
{
public:
  Data(int v) : mData(v)
  {
    mId = ++sIdCounter;
    print("Init    ",-1);
  }
  
  virtual ~Data()
  {
    print("Destroy ",-1);
  }
  
  // copy constructor
  Data(const Data& v)
  {
    mId    = ++sIdCounter;
    mData = v.mData;
    print("Copy    ",-1);
  }
  
  // const operation, no copy
  int value() const
  {
    return mData;
  }
  
  // non-const operation, make a copy
  void increment()
  {
    mData++;
  }
  
  void print(const char* msg, int ref_count) const
  { 
    if (ref_count > 0)
      printf("%s [%i] value:%i (%i)\n",msg,mId,mData,ref_count);
    else
      printf("%s [%i] value:%i\n",msg,mId,mData);
  }
  
private:
  static int sIdCounter;
  int mData;
  int    mId;
};

typedef SmartPtr<Data> Value;

int Data::sIdCounter = 0;

int main()
{
  Value v1(new Data(1));
  const Value v2(new Data(2));
  Value v3;
  
  printf("> v3 = v2\n");
  v3 = v2;
  
  printf("               ... done\n");
  v1->print("    v1 =",v1.ref_count());
  v2->print("    v2 =",v2.ref_count());
  v3->print("    v3 =",v3.ref_count());
  
  printf("> v1 = v2\n");
  v1 = v2;
  printf("               ... done\n");
  v1->print("    v1 =",v1.ref_count());
  v2->print("    v2 =",v2.ref_count());
  v3->print("    v3 =",v3.ref_count());
  
  printf("> v3.data()->value().\n");
  int i = v3.data()->value();
  printf("               ... %i\n",i);
  v1->print("    v1 =",v1.ref_count());
  v2->print("    v2 =",v2.ref_count());
  v3->print("    v3 =",v3.ref_count());
  
  printf("v3.mutable_data()->increment().\n");
  v3.mutable_data()->increment();
  printf("               ... done\n");
  v1->print("    v1 =",v1.ref_count());
  v2->print("    v2 =",v2.ref_count());
  v3->print("    v3 =",v3.ref_count());
  
}
