#ifndef APOLLON_FC_SRC_MACROS_H
#define APOLLON_FC_SRC_MACROS_H

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
#define _ARG_17(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, ...)    A17
#define _ARG_18(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, ...)    A18
#define _ARG_19(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19...)    A19
#define _ARG_20(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, ...)    A20
#define _ARG_21(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, ...)    A21
#define _ARG_22(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, ...)    A22
#define _ARG_23(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, ...)    A23
#define _ARG_24(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, ...)    A24
#define _ARG_25(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, ...)    A25
#define _ARG_26(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, ...)    A26
#define _ARG_27(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, ...)    A27
#define _ARG_28(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, ...)    A28
#define _ARG_29(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, ...)    A29
#define _ARG_30(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, ...)    A30
#define _ARG_31(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31, ...)    A31
#define _ARG_32(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31, A32, ...)    A32
#define _ARG_33(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31, A32, A33, ...)    A33
#define _ARG_34(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31, A32, A33, A34, ...)    A34
#define _ARG_35(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31, A32, A33, A34, A35, ...)    A35
#define _ARG_36(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31, A32, A33, A34, A35, A36, ...)    A36
#define _ARG_37(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31, A32, A33, A34, A35, A36, A37, ...)    A37
#define _ARG_38(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31, A32, A33, A34, A35, A36, A37, A38, ...)    A38
#define _ARG_39(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31, A32, A33, A34, A35, A36, A37, A38, A39, ...)    A39
#define _ARG_40(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24, A25, A26, A27, A28, A29, A30, A31, A32, A33, A34, A35, A36, A37, A38, A39, A40, ...)    A40

#define _MAP_1(F, L) F(ARG_N(0, L))
#define _MAP_2(F, L) _MAP_1(F, L);F(ARG_N(1, L))
#define _MAP_3(F, L) _MAP_2(F, L);F(ARG_N(2, L))
#define _MAP_4(F, L) _MAP_3(F, L);F(ARG_N(3, L))
#define _MAP_5(F, L) _MAP_4(F, L);F(ARG_N(4, L))
#define _MAP_6(F, L) _MAP_5(F, L);F(ARG_N(5, L))
#define _MAP_7(F, L) _MAP_6(F, L);F(ARG_N(6, L))
#define _MAP_8(F, L) _MAP_7(F, L);F(ARG_N(7, L))
#define _MAP_9(F, L) _MAP_8(F, L);F(ARG_N(8, L))
#define _MAP_10(F, L) _MAP_9(F, L);F(ARG_N(9, L))
#define _MAP_11(F, L) _MAP_10(F, L);F(ARG_N(10, L))
#define _MAP_12(F, L) _MAP_11(F, L);F(ARG_N(11, L))
#define _MAP_13(F, L) _MAP_12(F, L);F(ARG_N(12, L))
#define _MAP_14(F, L) _MAP_13(F, L);F(ARG_N(13, L))
#define _MAP_15(F, L) _MAP_14(F, L);F(ARG_N(14, L))
#define _MAP_16(F, L) _MAP_15(F, L);F(ARG_N(15, L))
#define _MAP_17(F, L) _MAP_16(F, L);F(ARG_N(16, L))
#define _MAP_18(F, L) _MAP_17(F, L);F(ARG_N(17, L))
#define _MAP_19(F, L) _MAP_18(F, L);F(ARG_N(18, L))
#define _MAP_20(F, L) _MAP_19(F, L);F(ARG_N(19, L))
#define _MAP_21(F, L) _MAP_20(F, L);F(ARG_N(20, L))
#define _MAP_22(F, L) _MAP_21(F, L);F(ARG_N(21, L))
#define _MAP_23(F, L) _MAP_22(F, L);F(ARG_N(22, L))
#define _MAP_24(F, L) _MAP_23(F, L);F(ARG_N(23, L))
#define _MAP_25(F, L) _MAP_24(F, L);F(ARG_N(24, L))
#define _MAP_26(F, L) _MAP_25(F, L);F(ARG_N(25, L))
#define _MAP_27(F, L) _MAP_26(F, L);F(ARG_N(26, L))
#define _MAP_28(F, L) _MAP_27(F, L);F(ARG_N(27, L))
#define _MAP_29(F, L) _MAP_28(F, L);F(ARG_N(28, L))
#define _MAP_30(F, L) _MAP_29(F, L);F(ARG_N(29, L))
#define _MAP_31(F, L) _MAP_30(F, L);F(ARG_N(30, L))
#define _MAP_32(F, L) _MAP_31(F, L);F(ARG_N(31, L))
#define _MAP_33(F, L) _MAP_32(F, L);F(ARG_N(32, L))
#define _MAP_34(F, L) _MAP_33(F, L);F(ARG_N(33, L))
#define _MAP_35(F, L) _MAP_34(F, L);F(ARG_N(34, L))
#define _MAP_36(F, L) _MAP_35(F, L);F(ARG_N(35, L))
#define _MAP_37(F, L) _MAP_36(F, L);F(ARG_N(36, L))
#define _MAP_38(F, L) _MAP_37(F, L);F(ARG_N(37, L))
#define _MAP_39(F, L) _MAP_38(F, L);F(ARG_N(38, L))
#define _MAP_40(F, L) _MAP_39(F, L);F(ARG_N(39, L))
#define __MAP_N(N, F, L) _MAP_##N(F, L)
#define _MAP_N(N, F, L) __MAP_N(N, F, L)

#define MAP(F, L) _MAP_N(SIZE_LIST(L), F, L)

#define _MAP_R_1(F, L) F(ARG_N(0, L))
#define _MAP_R_2(F, L) F(ARG_N(1, L));_MAP_R_1(F, L)
#define _MAP_R_3(F, L) F(ARG_N(2, L));_MAP_R_2(F, L)
#define _MAP_R_4(F, L) F(ARG_N(3, L))_MAP_R_3(F, L)
#define _MAP_R_5(F, L) F(ARG_N(4, L));_MAP_R_4(F, L)
#define _MAP_R_6(F, L) F(ARG_N(5, L));_MAP_R_5(F, L)
#define _MAP_R_7(F, L) F(ARG_N(6, L));_MAP_R_6(F, L)
#define _MAP_R_8(F, L) F(ARG_N(7, L));_MAP_R_7(F, L)
#define _MAP_R_9(F, L) F(ARG_N(8, L));_MAP_R_8(F, L)
#define _MAP_R_10(F, L) F(ARG_N(9, L));_MAP_R_9(F, L
#define _MAP_R_11(F, L) F(ARG_N(10, L));_MAP_R_10(F, L)
#define _MAP_R_12(F, L) F(ARG_N(11, L));_MAP_R_11(F, L)
#define _MAP_R_13(F, L) F(ARG_N(12, L));_MAP_R_12(F, L)
#define _MAP_R_14(F, L) F(ARG_N(13, L));_MAP_R_13(F, L)
#define _MAP_R_15(F, L) F(ARG_N(14, L));_MAP_R_14(F, L)
#define _MAP_R_16(F, L) F(ARG_N(15, L));_MAP_R_15(F, L)
#define _MAP_R_17(F, L) F(ARG_N(16, L));_MAP_R_16(F, L)
#define _MAP_R_18(F, L) F(ARG_N(17, L));_MAP_R_17(F, L)
#define _MAP_R_19(F, L) F(ARG_N(18, L));_MAP_R_18(F, L)
#define _MAP_R_20(F, L) F(ARG_N(19, L));_MAP_R_19(F, L)
#define _MAP_R_21(F, L) F(ARG_N(20, L));_MAP_R_20(F, L)
#define _MAP_R_22(F, L) F(ARG_N(21, L));_MAP_R_21(F, L)
#define _MAP_R_23(F, L) F(ARG_N(22, L));_MAP_R_22(F, L)
#define _MAP_R_24(F, L) F(ARG_N(23, L));_MAP_R_23(F, L)
#define _MAP_R_25(F, L) F(ARG_N(24, L));_MAP_R_24(F, L)
#define _MAP_R_26(F, L) F(ARG_N(25, L));_MAP_R_25(F, L)
#define _MAP_R_27(F, L) F(ARG_N(26, L));_MAP_r_26(F, L)
#define _MAP_R_28(F, L) F(ARG_N(27, L));_MAP_R_27(F, L)
#define _MAP_R_29(F, L) F(ARG_N(28, L));_MAP_R_28(F, L)
#define _MAP_R_30(F, L) F(ARG_N(29, L));_MAP_R_29(F, L)
#define _MAP_R_31(F, L) F(ARG_N(30, L));_MAP_R_30(F, L)
#define _MAP_R_32(F, L) F(ARG_N(31, L));_MAP_R_31(F, L)
#define _MAP_R_33(F, L) F(ARG_N(32, L));_MAP_R_32(F, L)
#define _MAP_R_34(F, L) F(ARG_N(33, L));_MAP_R_33(F, L)
#define _MAP_R_35(F, L) F(ARG_N(34, L));_MAP_R_34(F, L)
#define _MAP_R_36(F, L) F(ARG_N(35, L));_MAP_R_35(F, L)
#define _MAP_R_37(F, L) F(ARG_N(36, L));_MAP_R_36(F, L)
#define _MAP_R_38(F, L) F(ARG_N(37, L));_MAP_R_37(F, L)
#define _MAP_R_39(F, L) F(ARG_N(38, L));_MAP_R_38(F, L)
#define _MAP_R_40(F, L) F(ARG_N(39, L));_MAP_R_39(F, L)
#define __MAP_R_N(N, F, L) _MAP_R_##N(F, L)
#define _MAP_R_N(N, F, L) __MAP_R_N(N, F, L)

#define MAP_R(F, L) _MAP_R_N(SIZE_LIST(L), F, L)

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

#endif
