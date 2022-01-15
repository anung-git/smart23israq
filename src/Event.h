#ifndef Event_H
#define Event_H

#include <Arduino.h>
class Event
{
private:
    unsigned long previousMillis,interval ;    
public:
    Event(int milli_second);
    bool getEvent();
    void reset();
    void setEvent(int milli_second);
    ~Event();
};
// Existing code goes here

#endif