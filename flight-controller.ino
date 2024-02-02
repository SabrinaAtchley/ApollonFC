#define TEST_MODE

#ifdef TEST_MODE
#include "unit-tests.h"
#endif
#ifndef TEST_MODE
#include "main.h"
#endif
