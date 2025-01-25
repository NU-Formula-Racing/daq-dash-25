
#include <vector>
#include <stdint.h>
#include <string.h>

#include "serializer.hpp"

using namespace std;


vector<uint8_t> Serializer::saveData(CarData data) const
{
    vector<uint8_t> byteData;
    
    byteData.push_back(static_cast<uint8_t>(data.wheelBackLeftSpeed));
    byteData.push_back(static_cast<uint8_t>(data.wheelBackRightSpeed));
    byteData.push_back(static_cast<uint8_t>(data.wheelFrontLeftSpeed));
    byteData.push_back(static_cast<uint8_t>(data.wheelFrontRightSpeed));

    return byteData;
}


CarData Serializer::loadData(vector<uint8_t> data) const
{
    CarData intData;
    intData.wheelBackLeftSpeed = static_cast<int>(data[0]);
    intData.wheelBackRightSpeed = static_cast<int>(data[1]);
    intData.wheelFrontLeftSpeed = static_cast<int>(data[2]);
    intData.wheelFrontRightSpeed = static_cast<int>(data[3]);

    return intData;
}