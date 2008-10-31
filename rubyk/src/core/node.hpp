#ifndef _NODE_HPP_
#define _NODE_HPP_

/* Inline methods that could not live in the class to avoid circular definition */

inline void Node::bang_me_in (time_t pTime)
{
  BaseEvent * e = new BaseEvent(mServer->mCurrentTime + pTime, this); // sets mIsBang to true

  mServer->register_event( e );
}

inline void Node::loop_me ()
{
  mServer->register_looped_node(this);
  mLooped = true;
}


inline void Node::unloop_me ()
{
  if (mLooped && mServer) {
    mServer->free_looped_node(this);
    mLooped = false;
  }
}

inline void Node::remove_my_events ()
{
  if (mServer)
    mServer->free_events_for(this);
}

template <class T, void(T::*Tmethod)(void *)>
inline void Node::register_event (time_t pTime, void * data)
{
  BaseEvent * e = (BaseEvent *) new Event<T, Tmethod>(mServer->mCurrentTime + pTime, (T*)this, data);
  e->mForced = false;
  mServer->register_event( e );
}

template <class T, void(T::*Tmethod)(void *)>
inline void Node::register_forced_event (time_t pTime, void * data)
{
  BaseEvent * e = (BaseEvent *) new Event<T, Tmethod>(mServer->mCurrentTime + pTime, (T*)this, data);
  e->mForced = true;
  mServer->register_event( e );
}

#endif // _NODE_HPP_