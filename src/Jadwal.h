#ifndef Jadwal_H_
#define Jadwal_H_

#include <Arduino.h>
#include "Alamat.h"
#include "PrayerTimes.h"

class Jadwal
{
private:
#if defined(rumus)
    double times[sizeof(TimeName) / sizeof(char *)];
#endif // rumus
    float _bujur = 110.2163;
    float _lintang = -7.7959;
    unsigned char _zona = 7;
    uint8_t alarm_status;
    Alamat alamat = Alamat();
    unsigned char jadwal[14];
    char offsite[5];
    int _fix[5];
    void lookupJadwal(unsigned char tanggal, unsigned char bulan, int tahun);

public:
    Jadwal();
    ~Jadwal();
    uint8_t getAlarm();
    void setZona(unsigned char zona);
    void setAlarm(unsigned char alrm);
    void setTanggal(unsigned char tanggal, unsigned char bulan, int tahun);
    void setJam(unsigned char jam, unsigned char menit);

    void setBujur(float bujur);
    void setLintang(float lintang);

    void setOffsiteSubuh(char offsite);
    void setOffsiteDzuhur(char offsite);
    void setOffsiteAshar(char offsite);
    void setOffsiteMaghrib(char offsite);
    void setOffsiteIsya(char offsite);

    void setFixSubuh(int fix);
    void setFixDzuhur(int fix);
    void setFixAshar(int fix);
    void setFixMaghrib(int fix);
    void setFixIsya(int fix);

    // index (0=imsya,1=subuh,2=dzuhur,3=ashar,4=maghrb,5=isya,6=suruq,)
    bool getAlarmByOffsite(uint8_t index, uint8_t jam, uint8_t menit, int offsite);

    void getImsya(unsigned char &jam, unsigned char &menit);
    void getSubuh(unsigned char &jam, unsigned char &menit);
    void getSuruq(unsigned char &jam, unsigned char &menit);
    void getDzuhur(unsigned char &jam, unsigned char &menit);
    void getAshar(unsigned char &jam, unsigned char &menit);
    void getMaghrib(unsigned char &jam, unsigned char &menit);
    void getIsya(unsigned char &jam, unsigned char &menit);
};

#endif
