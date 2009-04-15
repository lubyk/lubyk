#ifndef _ORDERED_LIST_H_
#define _ORDERED_LIST_H_

#include <cstdlib>
#include <cstdio>
#include <vector>

template <class T>
struct LinkedList {
  LinkedList(T pObj) : obj(pObj), next(NULL) {}
  T obj;
  LinkedList* next;
};

/** This class maintains a linked list of elements, ensuring that the elements are kept sorted, 
  * the smallest being on top of the list. It also ensures that only one version of an element is stored.
  */
template <class T>
class OrderedList
{
  public:
    OrderedList()
    {
      to_node_p = new LinkedList<T>(NULL);
    }
    
    OrderedList(const OrderedList& other)
    {
      printf("YOU SHOULD NOT USE REFERENCES WITH ORDEREDLIST\n");
    }
    
    ~OrderedList()
    {
      LinkedList<T> * tmp;
      while( (tmp = to_node_p->next) ) {
        delete to_node_p;
        to_node_p = tmp;
      }
    }
    
    bool empty()
    { return !to_node_p->next; }

    T front()
    { return  to_node_p->next->obj; }
    
    bool get(T* pObj)
    {
      if ( empty() )
        return false;
      else {
        *pObj = to_node_p->next->obj;
        return true;
      } 
    }
    
    void push(T pObj)
    {
      LinkedList<T> * container = new LinkedList<T>(pObj);
      LinkedList<T> * iterator  = to_node_p;
      
      // make sure pObj is not in the list already
      while(iterator->next && pObj != iterator->next->obj) {
        iterator = iterator->next;
      }
      
      if (iterator->next && pObj == iterator->next->obj)
        return;
      
      iterator = to_node_p;
      
      // queue is kept sorted, smallest on top
      while(iterator->next && pObj >= iterator->next->obj) {
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
    
    LinkedList<T> * remove(T pObj)
    {
      LinkedList<T> * iterator = to_node_p;
      LinkedList<T> * tmp;
      
      // find element
      while(iterator->next && pObj != iterator->next->obj)
        iterator = iterator->next;
      
      if (iterator->next && pObj == iterator->next->obj) {
        // remove element
        tmp = iterator->next->next;
        delete iterator->next;
        iterator->next = tmp;
        return tmp;
      }
    }
    
    void pop()
    {
      LinkedList<T> * tmp;
      if (to_node_p->next) {
        tmp = to_node_p->next->next;
        delete to_node_p->next;
        to_node_p->next = tmp;
      }
    }
    
    LinkedList<T> * begin() { return to_node_p->next; }
    
    size_t size()
    {
      int i=0;
      LinkedList<T> * it = to_node_p;

      // find element
      while(it->next) {
        it = it->next;
        i++;
      }
      return i;
    }
    
  private:
    LinkedList<T> * to_node_p;
};

/** This class maintains a linked list of pointers to elements, ensuring that the elements are kept sorted, 
  * the smallest being on top of the list. It also ensures that only one pointer to an object is stored.
  */
template <class T>
class OrderedList<T*>
{
  public:
    OrderedList()
    {
      to_node_p = new LinkedList<T*>(NULL);
    }
    
    OrderedList(const OrderedList& other)
    {
      printf("YOU SHOULD NOT USE REFERENCES WITH ORDEREDLIST\n");
    }
    
    ~OrderedList()
    {
      LinkedList<T*> * tmp;
      while( (tmp = to_node_p->next) ) {
        delete to_node_p;
        to_node_p = tmp;
      }
    }
    
    bool empty()
    { return !to_node_p->next; }

    T *front()
    { return  to_node_p->next->obj; }
    
    bool get(T** pObj)
    {
      if ( empty() )
        return false;
      else {
        *pObj = to_node_p->next->obj;
        return true;
      } 
    }
    
    void push(T* pObj)
    {
      LinkedList<T*> * container = new LinkedList<T*>(pObj);
      LinkedList<T*> * iterator  = to_node_p;
      
      // make sure pObj is not in the list already
      while(iterator->next && pObj != iterator->next->obj) {
        iterator = iterator->next;
      }
      
      if (iterator->next && pObj == iterator->next->obj)
        return;
      
      iterator = to_node_p;
      
      // queue is kept sorted, smallest on top
      if (iterator->next) {
        while(iterator->next && *pObj >= *(iterator->next->obj)) {
          iterator = iterator->next;
        }
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
    
    LinkedList<T*> * remove(T* pObj)
    {
      LinkedList<T*> * iterator = to_node_p;
      LinkedList<T*> * tmp;
      
      // find element
      while(iterator->next && pObj != iterator->next->obj)
        iterator = iterator->next;
      
      if (iterator->next && pObj == iterator->next->obj) {
        // remove element
        tmp = iterator->next->next;
        delete iterator->next;
        iterator->next = tmp;
        return tmp;
      }
	  return to_node_p;
    }
    
    void pop()
    {
      LinkedList<T*> * tmp;
      if (to_node_p->next) {
        tmp = to_node_p->next->next;
        delete to_node_p->next;
        to_node_p->next = tmp;
      }
    }
    
    LinkedList<T*> * begin() { return to_node_p->next; }
    
    size_t size()
    {
      int i=0;
      LinkedList<T*> * it = to_node_p;

      // find element
      while(it->next) {
        it = it->next;
        i++;
      }
      return i;
    }
    
    /** Reorder a single element. */
    void sort_object(T *object) {
      remove(object);
      push(object);
    }
    
  private:
    LinkedList<T*> * to_node_p;
};

#endif