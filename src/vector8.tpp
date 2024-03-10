template<typename T>
bool vectorEquivalent(const T *l, const T *r, const uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    if (l[i] != r[i]) {
      return false;
    }
  }
  return true;
}

template<typename T>
bool vectorNotEquivalent(const T *l, const T *r, const uint8_t size) {
  return !vectorEquivalent(l, r, size);
}

template<typename T>
void vectorCopy(T *v, const T *vToCopy, const uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    v[i] = vToCopy[i];
  }
}

template<typename T>
void vectorAddition(T *l, const T *r, const uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    l[i] += r[i];
  }
}

template<typename T>
void vectorSubtract(T *l, const T *r, const uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    l[i] -= r[i];
  }
}

template<typename T>
void vectorScalarProduct(const T &s, T *r, const uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    r[i] *= s;
  }
}

template<typename T>
T vectorDotProduct(const T *l, const T *r, const uint8_t size) {
  T result = 0;

  for (uint8_t i = 0; i < size; i++) {
    result += l[i] * r[i];
  }

  return result;
}
