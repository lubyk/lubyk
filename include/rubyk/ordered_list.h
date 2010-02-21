/*
  ==============================================================================

   This file is part of the RUBYK project (http://rubyk.org)
   Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/

#ifndef RUBYK_SRC_CORE_ORDERED_LIST_H_
#define RUBYK_SRC_CORE_ORDERED_LIST_H_

#include <cstdlib>
#include <cstdio>
#include <vector>

#include "oscit/non_copyable.h"
  
template <class T>
struct LinkedList {
  LinkedList(T object) : obj(object), next(NULL) {}
  T obj;
  LinkedList* next;
};

/** This class maintains a linked list of elements, ensuring that the elements are kept sorted,
  * the smallest being on top of the list. It also ensures that only one version of an element is stored.
  */
template <class T>
class OrderedList : private oscit::NonCopyable {
  public:
    OrderedList() {
      linked_list_ = new LinkedList<T>(NULL);
    }

    ~OrderedList() {
      LinkedList<T> * tmp;
      while( (tmp = linked_list_->next) ) {
        delete linked_list_;
        linked_list_ = tmp;
      }
    }

    bool empty() {
      return !linked_list_->next;
    }

    T front() {
      return  linked_list_->next->obj;
    }

    bool get(T* object) {
      if ( empty() )
        return false;
      else {
        *object = linked_list_->next->obj;
        return true;
      }
    }

    void push(T object) {
      LinkedList<T> *container = new LinkedList<T>(object);
      push_container(container);
    }

    LinkedList<T> * remove(T object)
    {
      LinkedList<T> * iterator = linked_list_;
      LinkedList<T> * tmp;

      // find element
      while(iterator->next && object != iterator->next->obj)
        iterator = iterator->next;

      if (iterator->next && object == iterator->next->obj) {
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
      if (linked_list_->next) {
        tmp = linked_list_->next->next;
        delete linked_list_->next;
        linked_list_->next = tmp;
      }
    }

    LinkedList<T> * begin() { return linked_list_->next; }

    size_t size()
    {
      int i=0;
      LinkedList<T> * it = linked_list_;

      // find element
      while(it->next) {
        it = it->next;
        i++;
      }
      return i;
    }

    /** Reorder list in case an element's sort value
     *  changed.
     *  This is a simple implementation of 'insertion' sort.
     */
    void sort() {
      LinkedList<T> *old_head = linked_list_;
      LinkedList<T> *current, *next;

      linked_list_ = new LinkedList<T>(NULL);

      current = old_head->next;
      while(current) {
        next = current->next;
        current->next = NULL;
        push_container(current);
        current = next;
      }
      delete old_head;
    }

  protected:

    void push_container(LinkedList<T> *container) {
      LinkedList<T> * iterator  = linked_list_;

      // make sure object is not in the list already
      while(iterator->next && container->obj != iterator->next->obj) {
        iterator = iterator->next;
      }

      if (iterator->next && container->obj == iterator->next->obj)
        return;

      iterator = linked_list_;

      // queue is kept sorted, smallest on top
      while(iterator->next && container->obj >= iterator->next->obj) {
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

    LinkedList<T> * linked_list_;
};

/** This class maintains a linked list of pointers to elements, ensuring that the elements are kept sorted,
  * the smallest being on top of the list. It also ensures that only one pointer to an object is stored.
  */
template <class T>
class OrderedList<T*>
{
  public:
    OrderedList() {
      linked_list_ = new LinkedList<T*>(NULL);
    }

    OrderedList(const OrderedList& other)
    {
      printf("YOU SHOULD NOT USE REFERENCES WITH ORDEREDLIST\n");
    }

    ~OrderedList()
    {
      LinkedList<T*> * tmp;
      while( (tmp = linked_list_->next) ) {
        delete linked_list_;
        linked_list_ = tmp;
      }
    }

    bool empty()
    { return !linked_list_->next; }

    T *front()
    { return  linked_list_->next->obj; }

    bool get(T** object)
    {
      if ( empty() )
        return false;
      else {
        *object = linked_list_->next->obj;
        return true;
      }
    }

    void push(T* object) {
      LinkedList<T*> *container = new LinkedList<T*>(object);
      push_container(container);
    }

    LinkedList<T*> * remove(T* object)
    {
      LinkedList<T*> * iterator = linked_list_;
      LinkedList<T*> * tmp;

      // find element
      while(iterator->next && object != iterator->next->obj)
        iterator = iterator->next;

      if (iterator->next && object == iterator->next->obj) {
        // remove element
        tmp = iterator->next->next;
        delete iterator->next;
        iterator->next = tmp;
        return tmp;
      }
    return linked_list_;
    }

    void pop() {
      LinkedList<T*> * tmp;
      if (linked_list_->next) {
        tmp = linked_list_->next->next;
        delete linked_list_->next;
        linked_list_->next = tmp;
      }
    }

    LinkedList<T*> * begin() { return linked_list_->next; }

    size_t size()
    {
      int i=0;
      LinkedList<T*> * it = linked_list_;

      // find element
      while(it->next) {
        it = it->next;
        i++;
      }
      return i;
    }

    /** Reorder list in case an element's sort value
     *  changed.
     *  This is a simple implementation of 'insertion' sort.
     */
    void sort() {
      LinkedList<T*> *old_head = linked_list_;
      LinkedList<T*> *current, *next;

      linked_list_ = new LinkedList<T*>(NULL);

      current = old_head->next;
      while(current) {
        next = current->next;
        current->next = NULL;
        push_container(current);
        current = next;
      }
      delete old_head;
    }

  protected:

    void push_container(LinkedList<T*> *container) {
      LinkedList<T*> * iterator  = linked_list_;
      T *object = container->obj;

      // make sure object is not in the list already
      while(iterator->next && object != iterator->next->obj) {
        iterator = iterator->next;
      }

      if (iterator->next && object == iterator->next->obj)
        return;

      iterator = linked_list_;

      // queue is kept sorted, smallest on top
      if (iterator->next) {
        while(iterator->next && *object >= *(iterator->next->obj)) {
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

    LinkedList<T*> * linked_list_;
};

#endif // RUBYK_SRC_CORE_ORDERED_LIST_H_
