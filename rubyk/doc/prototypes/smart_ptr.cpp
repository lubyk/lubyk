/** 

compile and test with:

g++ smart_ptr.cpp -o test && ./test

*/

#include <cstdio>
#include "smart_ptr.h"

/** Signal types. */
enum value_t {
  NilValue  = 0,
  BangValue = 1, 
  NumberValue
};

class Number;

class Data
{
public:
  Data()
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
    print("Copy    ",-1);
  }
  
  virtual Data * clone()
  {
    printf("clone Data\n");
    return new Data(*this);
  }
  
  virtual value_t type() const
  { return BangValue; }
  
  virtual bool set (double * pResult)
  { return false; }

  virtual void print(const char* msg, int ref_count, const char* type_name = "") const
  { 
    if (ref_count > 0)
      printf("%s [%i %s] (%i)\n",msg,mId,type_name,ref_count);
    else
      printf("%s [%i %s]\n",msg,mId,type_name);
   }
  
protected:
  static int sIdCounter;
  int    mId;
};

class NumberData : public Data
{
public:
  NumberData(int n) : mData(n) {}
  
  // copy constructor
  NumberData(const NumberData& pOther)
  {
    mData = pOther.mData;
  }
  
  virtual Data * clone()
  {
    printf("clone NumberData\n");
    return new NumberData(*this);
  }
  
  virtual value_t type() const
  { return NumberValue; }
  
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

  void print(const char* msg, int ref_count, const char* type_name = "") const
  { 
    if (ref_count > 0)
      printf("%s [%i %s] value:%i (%i)\n",msg,mId,type_name,mData,ref_count);
    else
      printf("%s [%i %s] value:%i\n",msg,mId,type_name,mData);
  }
  
  virtual bool set (double * pResult)
  {
    *pResult = (double)mData;
    return true; 
  }
  
  virtual bool set (NumberData * pResult)
  {
    pResult->mData = mData;
    return true;
  }
private:  
  int mData;
};

int Data::sIdCounter = 0;

class Value : public SmartPtr<Data>
{
public:
  Value()
  {}
  
  Value(Data * p) : SmartPtr<Data>(p) 
  {}
  
  value_t type() const
  { return mPtr ? mPtr->mDataPtr->type() : NilValue; }
  
  virtual value_t holder_type() const
  { return NilValue; }
  
  const char* holder_type_name() const
  {
    switch (holder_type())
    {
      case NilValue:
      return "Nil   ";
      case BangValue:
      return "Bang  ";
      case NumberValue:
      return "Number";
      default:
      return "???";
    }
    
  }
  
  const char* type_name() const
  {
    switch (type())
    {
      case NilValue:
      return "Nil   ";
      case BangValue:
      return "Bang  ";
      case NumberValue:
      return "Number";
      default:
      return "???";
    }
  }
  
  bool set (Value * pResult) const
  {
    printf("I am a %s. I should set a %s\n",type_name(), pResult->holder_type_name());
    switch (type())
    {
      case NumberValue:
        switch (pResult->holder_type())
        {
          case NumberValue:
            *pResult = *this;
            return true;
          default:
            return false;
        }
      default:
        return false;
    }
  }
  
  bool set (double * pResult) const
  {
    if (!mPtr) return false;
    return mPtr->mDataPtr->set(pResult);
  }
  
  void print(const char* msg, int ref_count) const
  {
    if (!mPtr) return;
    mPtr->mDataPtr->print(msg, ref_count, type_name());
  }
};

class Number : public Value
{
public:
  Number() {}
  
  Number(NumberData * d) : Value(d) {}
  
  virtual value_t holder_type() const
  { return NumberValue; }
  
  /** This could blow up if mPtr is NULL... */
  const NumberData * operator-> () const
  {
    return (NumberData*)(mPtr->mDataPtr);
  }
  
  /** Return a const pointer to the data in the SmartPointer. */
  const NumberData * data () const
  {
    return mPtr ? (NumberData*)(mPtr->mDataPtr) : NULL;
  }
  
  /** Return a pointer to mutable data contained in the SmartPointer. Makes a copy if needed. */
  NumberData * mutable_data ()
  {
    if (!mPtr)
      return NULL;

    if (mPtr->mRefCount > 1)
      copy();

    return (NumberData*)(mPtr->mDataPtr);
  }
};



int main()
{
  const Value v1(new NumberData(1));
  Value v2(new Data());
  
  printf("> v3 = v1\n");
  Value v3 = v1;
  
  printf("               ... done\n");
  v1.print("    v1 =",v1.ref_count());
  v2.print("    v2 =",v2.ref_count());
  v3.print("    v3 =",v3.ref_count());
  
  printf("> v2 = v1\n");
  v2 = v1;
  printf("               ... done\n");
  v1.print("    v1 =",v1.ref_count());
  v2.print("    v2 =",v2.ref_count());
  v3.print("    v3 =",v3.ref_count());
  
  Number n;
  
  printf("> v3.set(&n)\n");
  if (v3.set(&n)) {
    printf("               ... ok\n");
  } else {
    printf("               ... fail\n");
    return -1;
  }
  
  printf("> n->value().\n");
  printf("               ... %i\n",n->value());
  v1.print("    v1 =",v1.ref_count());
  v2.print("    v2 =",v2.ref_count());
  v3.print("    v3 =",v3.ref_count());
  n.print( "    n  =", n.ref_count());
  
  printf("n.mutable_data()->increment().\n");
  n.mutable_data()->increment();
  printf("               ... done\n");
  v1.print("    v1 =",v1.ref_count());
  v2.print("    v2 =",v2.ref_count());
  v3.print("    v3 =",v3.ref_count());
  n.print( "    n  =", n.ref_count());
  
  
  printf("> v1.set(&d)\n");
  double d;
  
  if (v1.set(&d)) {
    printf("          %f   ... ok\n", d);
  } else {
    printf("               ... fail\n");
  }
  
  v1.print("    v1 =",v1.ref_count());
}
