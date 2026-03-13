//A6 Vlad Mihaescu, 110014634
#include <chrono>
#include <iostream>
#include <latch>
#include <print>
#include <random>
#include <syncstream>
#include <thread>

#include "concurrent_queue.hpp"
#include "cq_concepts.hpp"

using comp3400_2026w::concurrent_queue;
using comp3400_2026w::concurrent_queue_c;

static_assert(concurrent_queue_c<concurrent_queue<int>>);

comp3400_2026w::concurrent_queue<int> cq;

// have a latch to indicate when all threads can start working...
std::latch start_working(2);

// have a latch to indicate when all threads have finished working...
std::latch all_work_is_done(2);

// do_something() is a task that is concurrently done (i.e., more than
// one thread can be running this function)...
void do_something()
{
  using namespace std;

  random_device rd;
  default_random_engine int_re{rd()};
  default_random_engine bd_re{rd()};

  // use std::bernoulli_distribution to have a 50% probability of
  // pushing or popping...
  bernoulli_distribution bd{0.5};

  // use std::uniform_int_distribution to scale random values between
  // 0 and 999...
  uniform_int_distribution<int> ud(0,999);

  // wait for all threads to arrive here before starting to do work.
  start_working.arrive_and_wait();

  // run 25 milliseconds...
  auto time0{ std::chrono::steady_clock::now() };
  while (std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now() - time0).count() < 25)
  {
    if (bd(bd_re))
      cq.push(ud(int_re));
    else
    {
      // pop() outputs what was popped to std::cout in a way that is thread-safe
      // by using std::osyncstream...
      auto result{ cq.pop() };
      if (result)
        osyncstream(cout) << *result << ' ';
      else
        osyncstream(cout) << "<empty> ";
    }
  }

  // wait until all other threads have finished then quit...
  all_work_is_done.arrive_and_wait();
}

int main()
{
  using namespace std;

  // create two threads that do_something()...
  jthread t1(do_something);
  jthread t2(do_something);
  
  // wait until all thread work is done...
  t1.join();
  t2.join();

  // Output a message and quit...
  println("\n\nThere are {} elements remaining in the queue. They are: {}", cq.size(), cq);
}
