#ifndef FLIGHT_CONTROLLER_SRC_MACROS_H
#define FLIGHT_CONTROLLER_SRC_MACROS_H

// Use NUM_ARGS(__VA_ARGS__) to get the number of variadic arguments
#define _NUM_ARGS(_,n,m,l,k,j,i,h,g,f,e,d,c,b,a,Z,Y,X,W,V,U,T,S,R,Q,P,O,N,M,L,K,J,I,H,G,F,E,D,C,B,A,OUT,...) OUT
#define NUM_ARGS(V...) _NUM_ARGS(0,V,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
#define SIZE_LIST(LIST) NUM_ARGS LIST

#define ARG_N(N, LIST)       CAT(_ARG_, N) LIST
#define _ARG_0(A0, ...)      A0
#define _ARG_1(A0, A1, ...)  A1
#define _ARG_2(A0, A1, A2, ...)      A2
#define _ARG_3(A0, A1, A2, A3, ...)  A3
#define _ARG_4(A0, A1, A2, A3, A4, ...)      A4
#define _ARG_5(A0, A1, A2, A3, A4, A5, ...)  A5
#define _ARG_6(A0, A1, A2, A3, A4, A5, A6, ...)      A6
#define _ARG_7(A0, A1, A2, A3, A4, A5, A6, A7, ...)  A7
#define _ARG_8(A0, A1, A2, A3, A4, A5, A6, A7, A8, ...)      A8
#define _ARG_9(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, ...)  A9
#define _ARG_10(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, ...)    A10
#define _ARG_11(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, ...)    A11
#define _ARG_12(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, ...)    A12
#define _ARG_13(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, ...)    A13
#define _ARG_14(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, ...)    A14
#define _ARG_15(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, ...)    A15
#define _ARG_16(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, ...)    A16

#define SECOND(a,b,...)  b
#define BOOL_TEST(V...) SECOND(V, 0)

#define _DO_1(W,C,A)       _##W##_1(A)
#define _DO_2(W,C,A,B)     (_##W##_1(A) C _##W##_1(B))
#define _DO_3(W,C,A,V...)  (_##W##_1(A) C _DO_2(W,C,V))
#define _DO_4(W,C,A,V...)  (_##W##_1(A) C _DO_3(W,C,V))
#define _DO_5(W,C,A,V...)  (_##W##_1(A) C _DO_4(W,C,V))
#define _DO_6(W,C,A,V...)  (_##W##_1(A) C _DO_5(W,C,V))
#define _DO_7(W,C,A,V...)  (_##W##_1(A) C _DO_6(W,C,V))
#define _DO_8(W,C,A,V...)  (_##W##_1(A) C _DO_7(W,C,V))
#define __DO_N(W,C,N,V...) _DO_##N(W,C,V)
#define _DO_N(W,C,N,V...)  __DO_N(W,C,N,V)
#define DO(W,C,V...)       _DO_N(W,C,NUM_ARGS(V),V))

// Macros to support option testing
#define _CAT(a,V...) a##V
#define CAT(a,V...) _CAT(a,V)

#define _ISENA_     ~,1
#define _ISENA_1    ~,1
#define _ISENA_0x1  ~,1
#define _ISENA_true ~,1

#define _ENA_1(O)           BOOL_TEST(CAT(_IS, CAT(ENA_, O))
#define ENABLED(V...)       DO(ENA,&&,V)
#define ERROR_MESSAGE(c) CAT(ERROR_MESSAGE, c)

#define ERROR(c) BUZZER_ERROR_CODE(c)

#endif
