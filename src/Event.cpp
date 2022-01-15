#include "Event.h"
bool Event::getEvent(){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        return true;
    }
    else return false;
}


void Event::reset(){
    this->previousMillis=millis();
}
void Event::setEvent(int milli_second){
    this->interval = milli_second;
}

Event::Event(int milli_second)
{
    this->interval = milli_second;
    this->previousMillis=millis();
}

Event::~Event()
{
}
