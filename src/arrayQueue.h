#ifndef APOLLON_FC_SRC_ARRAY_QUEUE_H
#define APOLLON_FC_SRC_ARRAY_QUEUE_H

#include <Arduino.h>

/* Implements a queue-like data structure on top of a statically allocated
 * circular array.
 *
 * Pushing an element puts it after the "head". If the queue is full before
 *   pushing, it will delete the tail and then push.
 * Supports sequential reads, with getTail(), getHead(), and map(void f(const &T))
 */

template<typename T>
class ArrayQueue {
private:
  T* data;
  const uint8_t maxSize;
  uint8_t tail = 0, head;

public:
  uint8_t size = 0;

  // Initializes a new Circular Array, _data MUST BE EMPTY (contents will be overwritten)
  ArrayQueue(const T* _data, const uint8_t _maxSize);

  void append(const T v);
  void remove();
  void clear(); // Remove() all elements

  const T &getTail() const;
  const T &getHead() const;
  const T &getIndex(const uint8_t &idx) const; // Returns value at the given index, relative to tail
};

#include "arrayQueue.tpp"

#endif /* APOLLON_FC_SRC_ARRAY_QUEUE_H */
