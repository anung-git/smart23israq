#include "Jadwal.h"
Jadwal::Jadwal()
{
    set_calc_method(ISNA);
    set_asr_method(Shafii);
    set_high_lats_adjust_method(AngleBased);
    set_fajr_angle(20);
    set_isha_angle(18);
}

Jadwal::~Jadwal()
{
}

void Jadwal::setBujur(float bujur)
{
    _bujur = bujur;
}
void Jadwal::setLintang(float lintang)
{
    _lintang = lintang;
}

void Jadwal::setZona(unsigned char zona)
{
    _zona = zona;
}

void Jadwal::setFixSubuh(int fix)
{
    _fix[0] = fix;
}
void Jadwal::setFixDzuhur(int fix)
{
    _fix[1] = fix;
}
void Jadwal::setFixAshar(int fix)
{
    _fix[2] = fix;
}
void Jadwal::setFixMaghrib(int fix)
{
    _fix[3] = fix;
}
void Jadwal::setFixIsya(int fix)
{
    _fix[4] = fix;
}
void Jadwal::setOffsiteSubuh(char offsite)
{
    this->offsite[0] = offsite;
}
void Jadwal::setOffsiteDzuhur(char offsite)
{
    this->offsite[1] = offsite;
}
void Jadwal::setOffsiteAshar(char offsite)
{
    this->offsite[2] = offsite;
}
void Jadwal::setOffsiteMaghrib(char offsite)
{
    this->offsite[3] = offsite;
}
void Jadwal::setOffsiteIsya(char offsite)
{
    this->offsite[4] = offsite;
}
void Jadwal::lookupJadwal(unsigned char tanggal, unsigned char bulan, int tahun)
{
    uint16_t calc;
    int hours, minutes;
    get_prayer_times(tahun, bulan, tanggal, _lintang, _bujur, _zona, times);
    get_float_time_parts(times[0], hours, minutes);
    jadwal[2] = hours;
    jadwal[3] = minutes;
    calc = (jadwal[2] * 60) + jadwal[3] - 8; // imsyak = subuh - 10 menit (+2 toleransi)
    jadwal[0] = calc / 60;
    jadwal[1] = calc % 60;
    get_float_time_parts(times[1], hours, minutes);
    jadwal[12] = hours;
    jadwal[13] = minutes;
    get_float_time_parts(times[2], hours, minutes);
    jadwal[4] = hours;
    jadwal[5] = minutes;
    get_float_time_parts(times[3], hours, minutes);
    jadwal[6] = hours;
    jadwal[7] = minutes;
    get_float_time_parts(times[5], hours, minutes);
    jadwal[8] = hours;
    jadwal[9] = minutes;
    get_float_time_parts(times[6], hours, minutes);
    jadwal[10] = hours;
    jadwal[11] = minutes;

    // subuh
    calc = (jadwal[2] * 60) + jadwal[3] + 2 + offsite[0];
    if (_fix[0] > 0)
    {
        calc = _fix[0];
    }
    jadwal[2] = calc / 60;
    jadwal[3] = calc % 60;

    // dzuhur
    calc = (jadwal[4] * 60) + jadwal[5] + 2 + offsite[1];
    if (_fix[1] > 0)
    {
        calc = _fix[1];
    }
    jadwal[4] = calc / 60;
    jadwal[5] = calc % 60;
    // ashar
    calc = (jadwal[6] * 60) + jadwal[7] + 2 + offsite[2];
    if (_fix[2] > 0)
    {
        calc = _fix[2];
    }
    jadwal[6] = calc / 60;
    jadwal[7] = calc % 60;
    // maghrib
    calc = (jadwal[8] * 60) + jadwal[9] + 2 + offsite[3];
    if (_fix[3] > 0)
    {
        calc = _fix[3];
    }
    jadwal[8] = calc / 60;
    jadwal[9] = calc % 60;
    // isya
    calc = (jadwal[10] * 60) + jadwal[11] + 2 + offsite[4];
    if (_fix[4] > 0)
    {
        calc = _fix[4];
    }
    jadwal[10] = calc / 60;
    jadwal[11] = calc % 60;
}
bool Jadwal::getAlarmByOffsite(uint8_t index, uint8_t jam, uint8_t menit, int offsite)
{
    bool result = false;
    int time = (jam * 60) + menit + offsite;
    int compare;
    for (uint8_t i = 0; i < 7; i++)
    {
        if (i == index)
        {
            compare = (jadwal[i * 2] * 60) + jadwal[(i * 2) + 1];
            if (compare == time)
            {
                result = true;
            }
        }
    }
    return result;
}

uint8_t Jadwal::getAlarm()
{
    return alarm_status;
}
void Jadwal::setAlarm(unsigned char alrm)
{
    alarm_status = alrm;
}
void Jadwal::setJam(unsigned char jam, unsigned char menit)
{
    alarm_status = alamat.ALARM_OFF;
    if (jam == jadwal[0] && menit == jadwal[1])
    {
        alarm_status = alamat.ALARM_IMSYA;
    }
    if (jam == jadwal[2] && menit == jadwal[3])
    {
        alarm_status = alamat.ALARM_SUBUH;
    }
    if (jam == jadwal[12] && menit == jadwal[13])
    {
        alarm_status = alamat.ALARM_SURUQ;
    }
    if (jam == jadwal[4] && menit == jadwal[5])
    {
        alarm_status = alamat.ALARM_DZUHUR;
    }
    if (jam == jadwal[6] && menit == jadwal[7])
    {
        alarm_status = alamat.ALARM_ASHAR;
    }
    if (jam == jadwal[8] && menit == jadwal[9])
    {
        alarm_status = alamat.ALARM_MAGHRIB;
    }
    if (jam == jadwal[10] && menit == jadwal[11])
    {
        alarm_status = alamat.ALARM_ISYA;
    }
}

void Jadwal::setTanggal(unsigned char tanggal, unsigned char bulan, int tahun)
{
    this->lookupJadwal(tanggal, bulan, tahun);
}

void Jadwal::getImsya(unsigned char &jam, unsigned char &menit)
{

    jam = jadwal[0];
    menit = jadwal[1];
}
void Jadwal::getSubuh(unsigned char &jam, unsigned char &menit)
{
    jam = jadwal[2];
    menit = jadwal[3];
}
void Jadwal::getSuruq(unsigned char &jam, unsigned char &menit)
{
    jam = jadwal[12];
    menit = jadwal[13];
}
void Jadwal::getDzuhur(unsigned char &jam, unsigned char &menit)
{
    jam = jadwal[4];
    menit = jadwal[5];
}
void Jadwal::getAshar(unsigned char &jam, unsigned char &menit)
{
    jam = jadwal[6];
    menit = jadwal[7];
}
void Jadwal::getMaghrib(unsigned char &jam, unsigned char &menit)
{
    jam = jadwal[8];
    menit = jadwal[9];
}
void Jadwal::getIsya(unsigned char &jam, unsigned char &menit)
{
    jam = jadwal[10];
    menit = jadwal[11];
}