#include "testing_suite.hpp"
#include <serializer.cpp>

#include <vector>
#include <stdint.h>

using namespace std;

// a is a byte because its a char

void test_serializer()
{
    CarData data = {
        .wheelBackLeftSpeed = 70,
        .wheelBackRightSpeed = 71,
        .wheelFrontLeftSpeed = 70,
        .wheelFrontRightSpeed = 71
    };

    Serializer s;
    vector<uint8_t> bytes = s.saveData(data); // ideally write to a file

    CarData read = s.loadData(bytes); // ideally bytes is coming from a file

    TEST_ASSERT_TRUE(data == read);

}