#if !defined(BLUETOOTH_H)
#define BLUETOOTH_H

#include "Memory.h"
#include "Rtc.h"
#include "Buzer.h"
#include "power.h"
#include "Alamat.h"
#include "DFRobotDFPlayerMini.h"
#include <Arduino.h>
#include <EEPROM.h>
class Bluetooth
{
private:
    Alamat addres = Alamat();
    Memory *eprom;
    Rtc *rtc;
    DFRobotDFPlayerMini *myPlayer;
    Buzer *buzer;
    Power *power;
    enum CMDB
    {
        command_start = 1,
        set_jam,
        set_text,
        set_iqomah,
        set_tarhim,
        set_brightnes,
        set_offsite,
        set_fix,
        set_bt_kota,
        set_adzan,
        play_mp3,
        SET_POWER,
        SET_BUZER,
        RESET_PABRIK,
        command_end,
    };

    volatile unsigned char command;
    unsigned char status;

    void playManual(String &data);
    void setPower(String &data);
    void setTime(String &data);
    void setIqomah(String &data);
    void setAdzan(String &data);
    void setTarhim(String &data);
    void setOffsite(String &data);
    void setText(String &data);
    void setFix(String &data);
    void setBtKota(String &data);
    void setBuzer(String &data);
    void cmdStart(String &data);
    void setBrightness(String &data);
    void resetPabrik(String &data);

public:
    Bluetooth();
    ~Bluetooth();
    void setEprom(Memory *e);
    void setBuzer(Buzer *buzer);
    void setRtc(Rtc *rtc);
    void setPower(Power *power);
    void setPlayer(DFRobotDFPlayerMini *player);
    void loop();
    unsigned char available();
    unsigned char getStatus();
};

#endif // BLUETOOTH_H