/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
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

#ifndef LUBYK_INCLUDE_LUBYK_NODE_HPP_
#define LUBYK_INCLUDE_LUBYK_NODE_HPP_

namespace rk {

/* Inline methods that could not live in the class to avoid circular definition */

// template <class T, void(T::*Tmethod)(void *)>
// inline void Node::register_event (time_t pTime, void * data)
// {
//   BaseEvent * e = (BaseEvent *) new Event<T, Tmethod>(worker_->current_time_ + pTime, (T*)this, data);
//   e->forced_ = false;
//   worker_->register_event( e );
// }
// 
// template <class T, void(T::*Tmethod)(void *)>
// inline void Node::register_forced_event (time_t pTime, void * data)
// {
//   BaseEvent * e = (BaseEvent *) new Event<T, Tmethod>(worker_->current_time_ + pTime, (T*)this, data);
//   e->forced_ = true;
//   worker_->register_event( e );
// }
// 
} // rk

#endif // LUBYK_INCLUDE_LUBYK_NODE_HPP_