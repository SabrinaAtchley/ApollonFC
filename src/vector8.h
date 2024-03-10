#ifndef APOLLON_FC_SRC_VECTOR8_H
#define APOLLON_FC_SRC_VECTOR8_H

#include <Arduino.h>

/* Functions for using arrays as mathematical vectors
 * Supports vectors up to n = 255
 *
 */

template<typename T>
bool vectorEquivalent(const T *l, const T *r, const uint8_t size);

template<typename T>
bool vectorNotEquivalent(const T *l, const T *r, const uint8_t size);

template<typename T>
void vectorCopy(T *v, const T *vToCopy, const uint8_t size);

template<typename T>
void vectorAddition(T *l, const T *r, const uint8_t size);

template<typename T>
void vectorSubtract(T *l, const T *r, const uint8_t size);
template<typename T>
void vectorSubtraction(T *l, const T *r, const uint8_t size) {
  vectorSubtract(l, r, size);
}

template<typename T>
void vectorScalarProduct(const T &s, T *v, const uint8_t size);

template<typename T>
T vectorDotProduct(const T *l, const T *r, const uint8_t size);


#include "vector8.tpp"

#endif
