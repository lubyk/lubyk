#include <time.h>

#ifndef _H_RK_TIME_QUEUE_
#define _H_RK_TIME_QUEUE_

typedef struct rk_event_t_ rk_event_t;

struct rk_event_t_
{
  time_t when;
  void * receiver;
  void (*function)(void *);
  rk_event_t * next;
};

/** global event queue.
 The next event is gQueue->next.
*/
extern rk_event_t * gQueue;

/** global current time.
*/
extern time_t gCurrentTime;

void rk_add_to_queue      (time_t when, void* receiver, void (*function)(void *));
void rk_init_queue        ();
void rk_remove_from_queue (void* receiver);
void rk_run_queue         (time_t current_time);


#endif