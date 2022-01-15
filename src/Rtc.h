
#ifndef Rtc_H_
#define Rtc_H_

#include <Wire.h>
class Rtc
{
private:
    int _tahun, _intTime;
    unsigned char _detik, _menit, _jam, _hari;
    unsigned char _tanggal, _bulan;

    unsigned char makedec(unsigned char bcd);
    unsigned char makebcd(unsigned char dec);
    unsigned char calcHari(unsigned char date, unsigned char mount, int year);

public:
    Rtc(/* args */);
    ~Rtc();
    void readAll();
    void readDetik();
    void readMenit();
    void readJam();
    int getIntTime();
    void setJam(unsigned char hour, unsigned char min, unsigned char sec);
    void setTanggal(unsigned char date, unsigned char mount, int year);
    // void getTime(unsigned char &jam, unsigned char &menit, unsigned char &detik);
    // void getTanggal(unsigned char &hari,unsigned char &tanggal,unsigned char &bulan, int &tahun);

    // getter
    int getTahun();
    unsigned char getTanggal();
    unsigned char getBulan();
    unsigned char getHari();
    unsigned char getMenit();
    unsigned char getJam();
    unsigned char getDetik();
};

#endif
