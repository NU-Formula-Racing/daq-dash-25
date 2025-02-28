
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

    for (int i = 0; i < 4; i++)
    {
        pushValueToBytes(byteData, &data.wheelSpeeds[i], sizeof(float));
    }

    // addIntToBytes(byteData, data.wheelBackLeftSpeed);
    // addIntToBytes(byteData, data.wheelBackRightSpeed);
    // addIntToBytes(byteData, data.wheelFrontLeftSpeed);
    // addIntToBytes(byteData, data.wheelFrontRightSpeed);

    return byteData;
}


CarData Serializer::loadData(vector<uint8_t> data) const
{
    std::cout << "Serializer::loadData\n";
    CarData carData;
    size_t currentIndex = 0;

    for (int i = 0; i < 4; i++)
    {
        float *wheelSpeed = (float *)readValueFromBytes(data, &currentIndex, sizeof(float));
        carData.wheelSpeeds[i] = *wheelSpeed;
    }

    // // d ? ? ?

    // intData.wheelFrontRightSpeed = static_cast<int>(data[3]); // 4 thing in the array, and grab the next four bytes

    return carData;
}

void Serializer::pushValueToBytes(vector<uint8_t> &buffer, void* value, size_t size) const
{
    uint8_t *data = static_cast<uint8_t*>(value);
    for (size_t i = 0; i < size; i ++) {
        uint8_t byte = data[i];
        buffer.push_back(byte);
    }
}

void *Serializer::readValueFromBytes(vector<uint8_t> &buffer, size_t *current_index, size_t size) const {
    void *data = (void *)(&buffer[*current_index]);
    *current_index += size;
    return data;
}


// void Serializer::addIntToBytes(vector<uint8_t> &byteData, int num) const
// {
//     for (int i = 0; i < 4; i++)
//     {
//         byteData.push_back(num >> (i * 8));
//     }
// }
// // readIntFromBytes

// int Serializer::readIntFromBytes(vector<uint8_t> &byteData, int index) const
// {
//     int num = 0;

//     for (size_t i = 0; i < byteData.size(); i+=4)
//     {
//         num |= byteData[index]<<(i*8);
//     }

//     return num;
// }
