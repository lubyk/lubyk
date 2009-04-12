#ifndef _NODE_HPP_
#define _NODE_HPP_

/* Inline methods that could not live in the class to avoid circular definition */

// template <class T, void(T::*Tmethod)(void *)>
// inline void Node::register_event (time_t pTime, void * data)
// {
//   BaseEvent * e = (BaseEvent *) new Event<T, Tmethod>(mServer->current_time_ + pTime, (T*)this, data);
//   e->forced_ = false;
//   mServer->register_event( e );
// }
// 
// template <class T, void(T::*Tmethod)(void *)>
// inline void Node::register_forced_event (time_t pTime, void * data)
// {
//   BaseEvent * e = (BaseEvent *) new Event<T, Tmethod>(mServer->current_time_ + pTime, (T*)this, data);
//   e->forced_ = true;
//   mServer->register_event( e );
// }
// 
#endif // _NODE_HPP_