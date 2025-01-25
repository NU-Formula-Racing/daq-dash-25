#ifndef __TESTING_SUITE_H__
#define __TESTING_SUITE_H__

#include <unity.h>
#include <iostream>
#include <map>
#include <functional>

// test forward declarations
void test_serializer();

#define TEST_PAIR(func_name) { #func_name, func_name }

#define DEBUG_MSG(...) std::cout << __VA_ARGS__

class TestingSuite
{
public:
    TestingSuite() {}
    ~TestingSuite() {}

    void runTests()
    {
        UNITY_BEGIN();
        test_serializer();
        UNITY_END();
    }
};

#endif // __TESTING_SUITE_H__