#include "power.h"
Power::Power(/* args */)
{
}
// true = on, false = off
bool Power::getState()
{
    bool result;
    if (timeOn > timeOff)
    {
        if (timeNow >= timeOff && timeNow <= timeOn)
        {
            result = false;
        }
        else
        {
            result = true;
        }
    }
    else if (timeOn < timeOff)
    {
        if (timeNow >= timeOn && timeNow <= timeOff)
        {
            result = true;
        }
        else
        {
            result = false;
        }
    }
    else
    {
        result = true;
    }
    return result;
}
void Power::setTime(unsigned char hour, unsigned char min)
{
    timeNow = (hour * 60) + min;
}
void Power::setTimeOn(int time)
{
    timeOn = time;
}
void Power::setTimeOff(int time)
{
    timeOff = time;
}

