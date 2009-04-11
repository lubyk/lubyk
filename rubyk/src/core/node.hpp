#ifndef _NODE_HPP_
#define _NODE_HPP_

/* Inline methods that could not live in the class to avoid circular definition */

inline void Node::remove_my_events () {
  if (worker_) worker_->free_events_for(this);
}

inline void Node::bang_me_in(time_t interval) {
  worker_->register_event(new BangEvent(this, worker_->current_time_ + interval));
}

// inline void Node::loop_me ()
// {
//   mServer->register_looped_node(this);
//   mLooped = true;
// }
// 
// 
// inline void Node::unloop_me ()
// {
//   if (mLooped && mServer) {
//     mServer->free_looped_node(this);
//     mLooped = false;
//   }
// }
// 
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