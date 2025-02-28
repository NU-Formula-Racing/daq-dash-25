#ifndef __SERIALIZER_HPP__
#define __SERIALIZER_HPP__

#include <vector>
#include <stdint.h>
#include <string.h>

using namespace std;

struct CarData
{
public:
    float wheelSpeeds[4];
    float wheelTemps[32];
    float sus_displacement[4];
    float sg_measurements[4];
    float accel_vec[3];
    float pos_vec[3];

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

private:
    // void addIntToBytes(vector<uint8_t> &byteData, int num) const;
    // int readIntFromBytes(vector<uint8_t> &byteData, int index) consts;

    void pushValueToBytes(vector<uint8_t> &buffer, void* value, size_t size) const;
    void *readValueFromBytes(vector<uint8_t> &buffer, size_t *current_index, size_t size) const;
};


#endif // __SERIALIZER_HPP__