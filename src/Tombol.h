#if !defined(TOMBOL_H)
#define TOMBOL_H

#include <Arduino.h>

#include "Buzer.h"
class Tombol
{
private:
    unsigned char pos;
    Buzer *buzer; //= new Buzer(2);
    unsigned char _pinMenu, _pinUp, _pinDown;
    unsigned char waitUp = LOW;
    unsigned char waitDown = LOW;
    unsigned char waitMenu = LOW;
    int max, min, value;
    unsigned long DebounceMenu;
    unsigned long DebounceUp;
    unsigned long DebounceDown;
    const unsigned long debounce = 130;

public:
    Tombol(unsigned char pinMenu, unsigned char pinUp, unsigned char pinDown);
    ~Tombol();
    void loop();
    void setBuzer(Buzer *buzer);
    void setValue(int value);
    void setMax(int max);
    void setMin(int min);
    void resetMenu();
    int getValue();
    int getPos();
    bool getDownLongPress(unsigned long longDuration);
};

#endif // TOMBOL_H