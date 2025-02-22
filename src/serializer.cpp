
#include <vector>
#include <stdint.h>
#include <string.h>
#include <iostream>

#include <serializer.hpp>

using namespace std;


vector<uint8_t> Serializer::saveData(CarData data) const
{
    std::cout << "Serializer::saveData\n";
    vector<uint8_t> byteData;

    // int (4 bytes)
    // push(first)
    // push(second)
    // push(third)
    // push(fourth)

    addIntToBytes(byteData, data.wheelBackLeftSpeed);
    addIntToBytes(byteData, data.wheelBackRightSpeed);
    addIntToBytes(byteData, data.wheelFrontLeftSpeed);
    addIntToBytes(byteData, data.wheelFrontRightSpeed);

    return byteData;
}


CarData Serializer::loadData(vector<uint8_t> data) const
{
    std::cout << "Serializer::loadData\n";
    CarData intData;

    intData.wheelBackLeftSpeed = readIntFromBytes(data, 0);
    intData.wheelBackRightSpeed = readIntFromBytes(data, 4);
    intData.wheelFrontLeftSpeed = readIntFromBytes(data, 8);
    intData.wheelFrontRightSpeed = readIntFromBytes(data, 12);


    // // a b c d ? ? ? ? ?
    // intData.wheelBackLeftSpeed = static_cast<int>(data[0]);
    // intData.wheelBackRightSpeed = static_cast<int>(data[1]);
    // intData.wheelFrontLeftSpeed = static_cast<int>(data[2]);

    // // d ? ? ?

    // intData.wheelFrontRightSpeed = static_cast<int>(data[3]); // 4 thing in the array, and grab the next four bytes

    return intData;
}


void Serializer::addIntToBytes(vector<uint8_t> &byteData, int num) const
{
    for (int i = 0; i < 4; i++)
    {
        byteData.push_back(num >> (i * 8));
    }
}
// readIntFromBytes

int Serializer::readIntFromBytes(vector<uint8_t> &byteData, int index) const
{
    int num = 0;

    for (size_t i = 0; i < byteData.size(); i+=4)
    {
        num |= byteData[index]<<(i*8);
    }

    return num;

}
