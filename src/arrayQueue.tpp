
template<typename T>
ArrayQueue<T>::ArrayQueue(const T* _data, const uint8_t _maxSize)
  : data(_data), maxSize(_maxSize), head(_maxSize - 1) {}

template<typename T>
void ArrayQueue<T>::append(const T v) {
  head = (head + 1) % maxSize;

  if (head == tail) {
    remove();
  }

  data[head] = v;
  size++;
}

template<typename T>
void ArrayQueue<T>::remove() {
  if (size == 0) {
    return;
  }

  tail = (tail + 1) % maxSize;
  size--;
}

template<typename T>
void ArrayQueue<T>::clear() {
  tail = (head + 1) % maxSize;
  size = 0;
}

template<typename T>
const T &ArrayQueue<T>::getTail() const {
  return data[tail];
}

template<typename T>
const T &ArrayQueue<T>::getHead() const {
  return data[head];
}

template<typename T>
void ArrayQueue<T>::map(void f(const T&)) const {
  for (uint8_t i = tail; i < size; i++) {
    f(data[(i + tail) % maxSize]);
  }
}
