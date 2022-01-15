#if !defined(POWER_H)
#define POWER_H
class Power
{
private:
    unsigned int timeNow, timeOff, timeOn;

public:
    void setTime(unsigned char hour, unsigned char min); //(hour,min)
    void setTimeOn(int time);
    void setTimeOff(int time);
    bool getState();
    Power(/* args */);
};
#endif // POWER_H
