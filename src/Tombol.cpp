#include "Tombol.h"

int Tombol::getValue()
{
    return value;
}

int Tombol::getPos()
{
    return pos;
}
void Tombol::setValue(int value)
{
    this->value = value;
}
void Tombol::setMax(int max)
{
    this->max = max;
}
void Tombol::setMin(int min)
{
    this->min = min;
}

void Tombol::resetMenu()
{
    pos = 0;
}
void Tombol::loop()
{
    // tombol menu
    int menu = digitalRead(_pinMenu);
    if (menu == LOW)
    {
        unsigned int lastMillis = millis();
        if ((lastMillis - DebounceMenu) >= debounce)
        {
            DebounceMenu = lastMillis;
            if (waitMenu == LOW)
            {
                pos++;
                buzer->onOnce(150);
                // Serial.print("Menu = ");
                // Serial.println(pos);

                if (pos == 100)
                {
                    pos = 0;
                }
            }
            waitMenu = HIGH;
        }
    }
    else
    {
        waitMenu = LOW;
    }

    // tombol up
    int up = digitalRead(_pinUp);
    if (up == LOW)
    {
        unsigned int lastMillis = millis();
        if ((lastMillis - DebounceUp) >= debounce)
        {
            DebounceUp = lastMillis;

            if (waitUp == LOW)
            {
                value++;
                buzer->onOnce(150);
                if (value > max)
                {
                    value = min;
                }
                // Serial.print("value ");
                // Serial.println(value);

                if (pos == 0)
                {
                    pos = 100;
                }
            }
            waitUp = HIGH;
        }
    }
    else
    {
        waitUp = LOW;
    }
    // tombol down
    int down = digitalRead(_pinDown);
    if (down == LOW)
    {
        unsigned int lastMillis = millis();
        if ((lastMillis - DebounceDown) >= debounce)
        {
            DebounceDown = lastMillis;

            if (waitDown == LOW)
            {
                value--;
                buzer->onOnce(150);
                if (value < min)
                {
                    value = max;
                }
                // Serial.print("value ");
                // Serial.println(value);

                if (pos == 0)
                {
                    pos = 200;
                }
            }
            waitDown = HIGH;
        }
    }
    else
    {
        waitDown = LOW;
    }
    buzer->loop();
}
bool Tombol::getDownLongPress(unsigned long longDuration)
{
    unsigned long longPress = millis();
    while (1)
    {
        if (millis() - longPress >= longDuration)
        {
            return true;
        }
        int down = digitalRead(_pinDown);
        if (down == HIGH)
        {
            return false;
        }
        buzer->loop();
    }
}
void Tombol::setBuzer(Buzer *buzer)
{
    this->buzer = buzer;
}

Tombol::Tombol(unsigned char pinMenu, unsigned char pinUp, unsigned char pinDown)
{

    _pinMenu = pinMenu;
    _pinUp = pinUp;
    _pinDown = pinDown;
    pinMode(_pinMenu, INPUT_PULLUP);
    pinMode(_pinUp, INPUT_PULLUP);
    pinMode(_pinDown, INPUT_PULLUP);
    pos = 0;
}

Tombol::~Tombol()
{
}
