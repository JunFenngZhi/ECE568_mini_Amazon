#ifndef _ThreadSafe_queue_H
#define _ThreadSafe_queue_H

#include <mutex>
#include <queue>

using namespace std;

//Problem: push and pop will copy the object multiple times.

template<typename T>
class ThreadSafe_queue {
 private:
  queue<T> q;
  mutex lck;

 public:
  void push(const T & item) {
    lock_guard<mutex> lock(lck);
    q.push(item);
    
  }

  T pop() {
    lock_guard<mutex> lock(lck);
    T res = q.front();
    q.pop();
    return res;
  }
  size_T size() const() { return q.size(); }
  bool empty() const() { return q.empty(); }
  ThreadSafe_queue();
  ~ThreadSafe_queue();
};

#endif