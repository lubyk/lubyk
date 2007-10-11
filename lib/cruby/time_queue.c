// manage the event queue
#include "rubyk.h"

rk_event_t * gQueue;
time_t gCurrentTime;

void
rk_init_queue ()
{
  gQueue = rk_alloc(sizeof(rk_event_t));
  gQueue->when = 0; 
  gQueue->next = NULL;
}



/*
def add_to_queue(*args)
  time  = args[0]
  index = 0
  while(@@queue[index] && @@queue[index].time <= time) do
    index += 1
  end
  @@queue[index..index] = [Event.new(*args),@@queue[index]].compact
end
*/
void 
rk_add_to_queue (time_t when, void* receiver, void (*function)(void *))
{
  rk_event_t * event;
  rk_event_t * queue_ptr = gQueue;
  event = rk_alloc(sizeof(rk_event_t));
  event->when     = when;
  event->receiver = receiver;
  event->function = function;
  event->next     = NULL;
  
  // next event ('when' is the smallest) on top of queue
  while(queue_ptr->next && when >= queue_ptr->next->when) {
    queue_ptr = queue_ptr->next;
  }
  if (queue_ptr->next) {
    // insert between queue_ptr and queue_ptr->next
    event->next     = queue_ptr->next;
    queue_ptr->next = event;
  } else {
    // add to bottom of queue
    queue_ptr->next = event;
  }
}

/*
def remove_from_queue(obj)
  @@queue.reject! { |e| e.dest == obj}
end
*/
void
rk_remove_from_queue(void* receiver)
{
  rk_event_t * queue_ptr = gQueue;
  rk_event_t * tmp;
  while(queue_ptr->next) {
    if (queue_ptr->next->receiver == receiver) {
      // remove from queue
      tmp = queue_ptr->next;
      queue_ptr->next = queue_ptr->next->next;
      rk_free(tmp);
    } else {
      queue_ptr = queue_ptr->next;
    }
  }
}

/*
def run_queue(time)
  while @@queue[0] && @@queue[0].time <= time
    event = @@queue.shift
    event.trigger!
  end
end
*/
void
rk_run_queue(time_t current_time)
{
  rk_event_t * queue_ptr = gQueue->next;
  while(queue_ptr && queue_ptr->when <= current_time) {
    // set global time to queue_ptr->when
    gCurrentTime = queue_ptr->when;
    // call function ... 
    (*queue_ptr->function)(queue_ptr->receiver);
    // remove element from queue
    gQueue->next = queue_ptr->next;
    rk_free(queue_ptr);
    queue_ptr = gQueue->next;
  }
}