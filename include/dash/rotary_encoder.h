#include <string>
#include <vector>
#include "define.h"

class RotaryEncoder {
    public:
     void registerPush(std::function<void()> push); // *** TBA: argument should be lambda function
     void registerL(std::function<void()> left);
     void registerR(std::function<void()> right);
     void update();

    private:
    std::function<void()> push;
    std::function<void()> left;
    std::function<void()> right;

};