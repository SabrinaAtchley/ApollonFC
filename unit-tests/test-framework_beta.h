/*

*/
#include <iostream>
#include <exception>
#include <stdexcept>

#define _NUM_ARGS(_,n,m,l,k,j,i,h,g,f,e,d,c,b,a,Z,Y,X,W,V,U,T,S,R,Q,P,O,N,M,L,K,J,I,H,G,F,E,D,C,B,A,OUT,...) OUT
#define NUM_ARGS(V...) _NUM_ARGS(0,V,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)


#define _echo_1(a) a
#define _echo_2(a, b) a << b
#define _echo_3(a, V...) a << _echo_2(V)
#define _echo_4(a, V...) a << _echo_3(V)
#define _echo_5(a, V...) a << _echo_4(V)
#define _echo_6(a, V...) a << _echo_5(V)
#define _echo_7(a, V...) a << _echo_6(V)
#define _echo_8(a, V...) a << _echo_7(V)
#define _echo_9(a, V...) a << _echo_8(V)
#define _echo_10(a, V...) a << _echo_9(V)
#define __echo_n(N, V...) _echo_##N(V)
#define _echo_n(N, V...) __echo_n(N, V)
#define _echo(V...) _echo_n(NUM_ARGS(V), V)

#define PRINTLN(V...) std::cout << _echo(V) << std::endl

#define TEST_UNIT(n, b) [&]() {PRINTLN("Test unit ", n); int __test_passes = 0; int __test_fails = 0; const char* __prefix = "\t";b;return __test_fails;}()

#define _TEST_SECTION_END() PRINTLN("\n", __prefix, "Results: ", __test_passes, " passes/", __test_fails, " fails")

#define TEST_SECTION(n, b) [&]() {PRINTLN(__prefix, "Section: ", n);int __test_passes = 0;int __test_fails = 0; const char* __prefix = "\t\t";b;_TEST_SECTION_END();return __test_fails;}()

#define REQUIRE(n, c) if (c) {__test_passes += 1; PRINTLN(__prefix, "Test ", #n, "(R): passed");} else {__test_fails += 1; PRINTLN(__prefix, "Test ", #n, "(R): failed"); _TEST_SECTION_END(); return __test_fails;}
#define REQUIRE_FALSE(n, c) if (c) {__test_fails += 1; PRINTLN(__prefix, "Test ", #n, "(R): failed");_TEST_SECTION_END();return __test_fails;} else {__test_passes += 1; PRINTLN(__prefix, "Test ", #n, "(R): passed");}
#define REQUIRE_THROWS(n, f) try {f;PRINTLN(__prefix,"Exception expected for test ", #n, ", none thrown.");__test_fails += 1;_TEST_SECTION_END();return __test_fails;} catch (...) {__test_passes += 1;PRINTLN(__prefix, "Test ", #n, "(E): passed");}
#define REQUIRE_NOTHROWS(n, f) try{f;__test_passes += 1;PRINTLN(__prefix, "Test ", #n, "(E): passed");} catch(std::exception& e){PRINTLN(__prefix, "Exception ", e.what(), " thrown for test ", #n);__test_fails += 1;_TEST_SECTION_END();return __test_fails;} catch(...){PRINTLN(__prefix, "Unknown exception thrown for test ", #n);__test_fails += 1;_TEST_SECTION_END();return __test_fails;}
#define TEST_CASE(n, c) if (c) {__test_passes += 1; PRINTLN(__prefix, "Test ", #n, ": passed");} else {__test_fails += 1; PRINTLN(__prefix, "Test ", #n, ": failed");}
#define TEST_FALSE(n, c) if (c) {__test_fails += 1; PRINTLN(__prefix, "Test ", #n, ": failed");} else {__test_passes += 1; PRINTLN(__prefix, "Test ", #n, ": passed");}

int main() {
    TEST_UNIT("Vector16", {
        TEST_SECTION("Length constructor", {
            REQUIRE_FALSE(1.1, false);
            REQUIRE_THROWS(1.2, throw std::runtime_error("foobar"));
            TEST_CASE(1.3, 1+1 == 2);
        });
    });
}
