#include <stdlib.h>
#include <stdio.h>

// don't use complex objects as argument types and return values (use references instead),
// don't use a virtual method as a target for delegates (because usually it may not be inlined),
// put a target method implementation and 
//  creation code into the same compilation unit.

class Trigger
{
  public:
    Trigger () : receiver_(NULL), mFunction(NULL) {}

    template <class T, void(T::*Tmethod)(void)>
    static Trigger new_for (T* receiver)
    {
      Trigger t;
      t.receiver_ = receiver;
      t.mFunction = &cast_method<T, Tmethod>;
      return t;
    }
  
    virtual ~Trigger () { }

    /* PUBLIC METHODS */

    void run ()
    {
      return (*mFunction)(receiver_);
    }

  private:
    /* ATTRIBUTES */
    void* receiver_;
    void (*mFunction)(void* receiver);

  
    /* PRIVATE METHODS */
  
    /** This template method lets us cast back the receiver (void*) to it's proper type, thus enabling the
      * call on it's member function properly. */
    template <class T, void (T::*Tmethod)(void)>
    static void cast_method (void * receiver)
    {
      printf("Casted [%p]\n", Tmethod);
      (((T*)receiver)->*Tmethod)();
    }
};

class 
Node {
  public:
    Node (int pCounter) : mCounter(pCounter) { };
    virtual ~Node() {};

    /* PUBLIC METHODS */
    Trigger getTrigger () { 
      printf("Get [%p]\n", &Node::increment_counter);
      return Trigger::new_for<Node, &Node::increment_counter>(this); }
    void increment_counter (void) { mCounter++; }
    int  getCounter        (void) { return mCounter;   }


  private:
    /* ATTRIBUTES */
    int mCounter;
};

class 
SubNode : public Node {
  public:
    SubNode (int pCounter) : Node(pCounter) { };
    virtual ~SubNode() {};

    /* PUBLIC METHODS */
    Trigger getTrigger () { return Trigger::new_for<SubNode, &SubNode::increment_counter>(this); }
    void increment_counter (void) { mCounter += 2; }
    int  getCounter        (void) { return mCounter;   }


  private:
    /* ATTRIBUTES */
    int mCounter;
};

int main () {
  Node    node(5);
  SubNode sub (0);
  Trigger caller1 = node.getTrigger();
  Trigger caller2 = sub.getTrigger();
  
  printf("Initial node  : %i\n", node.getCounter());
  printf("Initial sub   : %i\n", sub.getCounter());
  caller1.run();
  caller2.run();
  printf("Node, after one run : %i\n", node.getCounter());
  printf("Sub,  after one run : %i\n", sub.getCounter());
  return 0;
}