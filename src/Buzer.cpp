#include "Buzer.h"
void Buzer::setOn()
{
    digitalWrite(pin, HIGH);
}
void Buzer::setOff()
{
    digitalWrite(pin, LOW);
}
void Buzer::onOnce(unsigned long duration)
{
    aktif = true;
    digitalWrite(pin, HIGH);
    this->duration = duration + millis();
}

void Buzer::onRepeat(uint8_t repeat, unsigned long duration)
{
    if (repeat > 100)
    {
        repeat = 100;
    }
    interval = duration;
    this->repeatduration = repeat * duration * 2;
    this->repeatduration += millis();
}
void Buzer::loop()
{
    unsigned long mls = millis();
    if (aktif == true)
    {
        if (mls > duration)
        {
            aktif = false;
            digitalWrite(pin, LOW);
        }
    }

    if (repeatduration > mls)
    {
        if ((repeatduration - mls) % (2 * interval) < interval)
        {
            digitalWrite(pin, LOW);
        }
        else
        {
            digitalWrite(pin, HIGH);
        }
    }
}

Buzer::Buzer(int pin)
{
    pinMode(pin, OUTPUT);
    this->pin = pin;
}

Buzer::~Buzer()
{
}
