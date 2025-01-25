#ifndef __SERIALIZER_HPP__
#define __SERIALIZER_HPP__

#include <vector>
#include <stdint.h>
#include <string.h>

using namespace std;

struct CarData
{
public:
    int wheelBackLeftSpeed;
    int wheelBackRightSpeed;
    int wheelFrontLeftSpeed;
    int wheelFrontRightSpeed;

    // for testing purposes, ignore
    bool operator==(const CarData &other) const {
        return (bool)memcmp(
            this,
            &other,
            sizeof(CarData)
        );
    }
};

class Serializer
{
public:
    vector<uint8_t> saveData(CarData data) const;
    CarData loadData(vector<uint8_t> data) const;

};

#endif // __SERIALIZER_HPP__