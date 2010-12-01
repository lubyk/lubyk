#include <libkern/OSAtomic.h>

#include <pthread.h>
#include <iostream>


#define THREAD_COUNT 100
#define LOOP_SIZE 10000

class Counter {
public:
  Counter(int val = 0) : count_(val) {}

  virtual void increment() {
    ++count_;
  }

  /** Decrement the counter by one and return true iff the
   * new value is zero.
   */
  virtual bool decrement() {
    return --count_ == 0;
  }

  __attribute__((__aligned__(4))) int32_t count_;
};

class AtomicCounter : public Counter {
public:
  AtomicCounter(int val = 0) : Counter(val) {}

  virtual void increment() {
    OSAtomicIncrement32(&count_);
  }

  virtual bool decrement() {
    return OSAtomicDecrement32(&count_) == 0;
  }
};

static void *play_with_counter( void *data ) {
  AtomicCounter *counter = (AtomicCounter*)data;
  for (int i=0; i < LOOP_SIZE; ++i) {
    counter->increment();
    counter->decrement();
  }
}

static void play(Counter &counter) {

  std::cout << "start: " << counter.count_ << "\n";

  // create n threads that will increment and decrement the counter
  pthread_t thread[THREAD_COUNT];

  for (size_t i=0; i < THREAD_COUNT; ++i) {
    pthread_create( &thread[i], NULL, play_with_counter, (void*)&counter);
  }

  for (size_t i=0; i < THREAD_COUNT; ++i) {
    pthread_join(thread[i], NULL);
  }

  std::cout << "end  : " << counter.count_ << "\n";
}

int main() {
  Counter counter;
  AtomicCounter atomic;

  std::cout << "SIMPLE COUNTER " << THREAD_COUNT << " threads\n";
  play(counter);

  std::cout << "\n\nATOMIC COUNTER " << THREAD_COUNT << " threads\n";
  play(atomic);
}
