#include <string>
#include <vector>
#include "define.h"

class RotaryEncoder {
    public:
     void registerPush(std::function<void()> cb); // *** TBA: argument should be lambda function
     void registerL(std::function<void()> cb);
     void registerR(std::function<void()> cb);
     void update();

    private:
    std::function<void()> onPush;
    std::function<void()> onLeft;
    std::function<void()> onRight;

    int lastA = 0;
    int lastB = 0;
    int lastButton = 0;
};