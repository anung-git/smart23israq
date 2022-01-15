
#include "Rtc.h"

// senin 1
// selasa 2
// rabu 3
// kamis 4
// jumat 5
// sabtu 6
// minggu 7
unsigned char Rtc::makedec(unsigned char bcd)
{
    return ((bcd / 16 * 10) + (bcd % 16));
}
unsigned char Rtc::makebcd(unsigned char dec)
{
    return ((dec / 10 * 16) + (dec % 10));
}

void Rtc::readAll()
{
    Wire.beginTransmission(104);
    Wire.write(0); // MSB
    Wire.endTransmission();
    if (Wire.requestFrom(104, 7) == 7)
    {
        _detik = Rtc::makedec(Wire.read());
        _menit = Rtc::makedec(Wire.read());
        _jam = Rtc::makedec(Wire.read());
        _hari = Rtc::makedec(Wire.read());
        _tanggal = Rtc::makedec(Wire.read());
        _bulan = Rtc::makedec(Wire.read());
        _tahun = Rtc::makedec(Wire.read()) + 2000;
        _intTime = _jam * 60 + _menit;
    }
}
void Rtc::readDetik()
{
    Wire.beginTransmission(104);
    Wire.write(0); // MSB
    Wire.endTransmission();
    if (Wire.requestFrom(104, 1) == 1)
    {
        _detik = Rtc::makedec(Wire.read());
    }
}
void Rtc::readMenit()
{
    Wire.beginTransmission(104);
    Wire.write(1); // MSB
    Wire.endTransmission();
    if (Wire.requestFrom(104, 1) == 1)
    {
        _menit = Rtc::makedec(Wire.read());
        _intTime = _jam * 60 + _menit;
    }
}
void Rtc::readJam()
{
    Wire.beginTransmission(104);
    Wire.write(2); // MSB
    Wire.endTransmission();
    if (Wire.requestFrom(104, 1) == 1)
    {
        _jam = Rtc::makedec(Wire.read());
        _intTime = _jam * 60 + _menit;
    }
}
// void Rtc::getTime(unsigned char &jam, unsigned char &menit, unsigned char &detik)
// {
//     Wire.beginTransmission(104);
//     Wire.write(0); // MSB
//     Wire.endTransmission();
//     if (Wire.requestFrom(104, 3) == 3)
//     {
//         detik = Rtc::makedec(Wire.read());
//         menit = Rtc::makedec(Wire.read());
//         jam = Rtc::makedec(Wire.read());
//     }
// }

unsigned char Rtc::calcHari(unsigned char d, unsigned char m, int y)
{
    const unsigned char t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    y -= m < 3;
    unsigned char h = (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
    if (h == 0)
        h = 7;
    return h;
}
void Rtc::setJam(unsigned char hour, unsigned char min, unsigned char sec)
{
    _jam = hour;
    _menit = min;
    _detik = sec;
    Wire.beginTransmission(104);
    Wire.write(0);
    Wire.write(Rtc::makebcd(sec));
    Wire.write(Rtc::makebcd(min));
    Wire.write(Rtc::makebcd(hour));
    Wire.endTransmission();
    Wire.beginTransmission(104);
    Wire.write(0x0e);
    Wire.write(0); // enable sqw 1hz
    Wire.endTransmission();
}

void Rtc::setTanggal(unsigned char date, unsigned char mount, int year)
{
    unsigned char _hari = Rtc::calcHari(date, mount, year);
    _tanggal = date;
    _bulan = mount;
    _tahun = year;
    this->_hari = _hari;

    Wire.beginTransmission(104);
    Wire.write(3);
    Wire.write(makebcd(_hari));
    Wire.write(makebcd(date));
    Wire.write(makebcd(mount));
    Wire.write(makebcd(year - 2000));
    Wire.endTransmission();
    Wire.beginTransmission(104);
    Wire.write(0x0e);
    Wire.write(0); // enable sqw 1hz
    Wire.endTransmission();
}
// void Rtc::getTanggal(unsigned char &hari, unsigned char &tanggal, unsigned char &bulan, int &tahun)
// {
//     Wire.beginTransmission(104);
//     Wire.write(3); // MSB
//     Wire.endTransmission();
//     if (Wire.requestFrom(104, 4) == 4)
//     {
//         hari = Rtc::makedec(Wire.read());
//         tanggal = Rtc::makedec(Wire.read());
//         bulan = Rtc::makedec(Wire.read());
//         tahun = Rtc::makedec(Wire.read()) + 2000;
//     }
// }

int Rtc::getIntTime()
{
    return _intTime;
}
int Rtc::getTahun()
{
    return _tahun;
}
unsigned char Rtc::getBulan()
{
    return _bulan;
}
unsigned char Rtc::getTanggal()
{
    return _tanggal;
}
unsigned char Rtc::getHari()
{
    return _hari;
}
unsigned char Rtc::getJam()
{
    return _jam;
}
unsigned char Rtc::getMenit()
{
    return _menit;
}
unsigned char Rtc::getDetik()
{
    return _detik;
}
Rtc::Rtc()
{
    Wire.begin();
}

Rtc::~Rtc()
{
}
