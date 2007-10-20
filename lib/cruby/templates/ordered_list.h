#ifndef _ORDERED_LIST_H_
#define _ORDERED_LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <vector>

template <class T>
struct LinkedList {
  LinkedList(T* pObj) : obj(pObj), next(NULL) {}
  T* obj;
  LinkedList* next;
};

/** This class maintains a linked list of pointers to elements, ensuring that the elements are kept sorted, 
  * the smallest being on top of the list. It also ensures that only one pointer to an object is stored.
  */
template <class T>
class OrderedList
{
  public:
    OrderedList()
    {
      mTop = new LinkedList<T>(NULL);
    }
    
    ~OrderedList()
    {
      LinkedList<T> * tmp;
      while(tmp = mTop->next) {
        delete mTop;
        mTop = tmp;
      }
    }
    
    T* top()
    { return  mTop->next ? mTop->next->obj : NULL; }
    
    void push(T* pObj)
    {
      LinkedList<T> * container = new LinkedList<T>(pObj);
      LinkedList<T> * iterator  = mTop;
      
      // make sure pObj is not in the list already
      while(iterator->next && pObj != iterator->next->obj)
        iterator = iterator->next;
      
      if (iterator->next && pObj == iterator->next->obj)
        return;
        
      iterator = mTop;
      
      // queue is kept sorted, smallest on top
      while(iterator->next && *pObj >= *(iterator->next->obj)) {
        iterator = iterator->next;
      }
      
      if (iterator->next) {
        // insert between iterator and iterator->next
        container->next = iterator->next;
        iterator->next  = container;
      } else {
        // add to bottom of queue
        iterator->next = container;
      }
    }
    
    void remove(T* pObj)
    {
      LinkedList<T> * iterator = mTop;
      LinkedList<T> * tmp;
      
      // find element
      while(iterator->next && pObj != iterator->next->obj)
        iterator = iterator->next;
      
      if (iterator->next && pObj == iterator->next->obj) {
        // remove element
        tmp = iterator->next->next;
        delete iterator->next;
        iterator->next = tmp;
      }
    }
    
    void pop()
    {
      LinkedList<T> * tmp;
      if (mTop->next) {
        tmp = mTop->next->next;
        delete mTop->next;
        mTop->next = tmp;
      }
    }
    
    LinkedList<T> * begin() { return mTop->next; }
    
  private:
    LinkedList<T> * mTop;
};

#endif