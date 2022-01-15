#include "Segmen.h"
#include <avr/pgmspace.h>
#include <Arduino.h>

// Segmen::Segmen(const int data, const int sck, const int strobe)
Segmen::Segmen()
{
    Segmen::alarm = 0;
    pinMode(PIN_PA4, OUTPUT);
    pinMode(PIN_PA5, OUTPUT);
    pinMode(PIN_PA6, OUTPUT);
    for (unsigned char i = 0; i < 4; i++)
    {
        Segmen::buffer[i] = dataJam[0];
        layer[i] = ON;
    }
    for (unsigned char i = 4; i < 14; i++)
    {
        Segmen::buffer[i] = dataKalender[0];
        layer[i] = ON;
    }
    for (unsigned char i = 12; i < 42; i++)
    {
        Segmen::buffer[i] = dataJadwal[0];
        layer[i] = ON;
    }
    this->loop();
}
unsigned char Segmen::bagiSepuluh(unsigned char nilai)
{
    return (nilai / 10);
}
unsigned char Segmen::sisaBagiSepuluh(unsigned char nilai)
{
    return (nilai % 10);
}
void Segmen::setTime(unsigned char hour, unsigned char min)
{
    Segmen::buffer[0] = (Segmen::dataJam[bagiSepuluh(hour)]);
    Segmen::buffer[1] = (Segmen::dataJam[sisaBagiSepuluh(hour)]);
    Segmen::buffer[2] = (Segmen::dataJam[bagiSepuluh(min)]);
    Segmen::buffer[3] = (Segmen::dataJam[sisaBagiSepuluh(min)]);
}
void Segmen::setSqw(bool value)
{
    sqw = value;
}
void Segmen::displaySholat(unsigned char sholat)
{
    const unsigned char lookupSholat[100] = {
        k_, k_, ki, km1, km2, ks, ky, ka, k_, k_,
        k_, k_, ks, ku, kb, ku, kh, k_, k_, k_,
        k_, k_, ks, ku, kr, ku, kq, k_, k_, k_,
        k_, k_, kd, ku, kh, ku, kr, k_, k_, k_,
        k_, k_, kj, ku, km1, km2, ka, kt, k_, k_,
        k_, k_, ka, ks, kh, ka, kr, k_, k_, k_,
        k_, km1, km2, ka, kg, kh, kr, ki, kb, k_,
        k_, k_, k_, ki, ks, ky, ka, k_, k_, k_,
        k_, ki, kq, ko, km1, km2, ka, kh, k_, k_,
        kk, kh, ku, kt, kb, ka, kh, k_, k_, k_,
        // k_, ks, ke, kn, ki, kn, k_, k_,
        // k_, ks, ke, kl, ka, ks, ka, k_,
        // k_, k_, kr, ka, kb, ku, k_, k_,
        // k_, kk, ka, km1, km2, ki, ks, k_,
        // k_, kj, ku, km1, km2, ka, kt, k_,
        // k_, ks, ka, kb, kt, ku, k_, k_,
        // k_, k_, ka, kh, ka, kd, k_, k_,
        // k_, ka, kl, ka, kr, km1, km2, k_,
        // k_, k_, k_, ko, kn, k_, k_, k_,
        // k_, k_, k_, ko, kf, kf, k_, k_,
        // k_, ks, kt, ka, kn, kb, ky, k_,
        // k_, k_, kk, ko, kt, ka, k_, k_,
        // k_, kj, ka, kd, kw1, kw2, ka, kl,
        // kt, ka, kr, kt, ki, kl, k_, 237,
        // kt, ka, kr, kt, ki, kl, k_, 67,
        // kt, ka, kr, kt, ki, kl, k_, 73,
        // kt, ka, kr, kt, ki, kl, k_, 45,
        // kt, ka, kr, kt, ki, kl, k_, 25,
        // kt, ka, kr, kt, ki, kl, k_, 17,
        // k_, k_, k_, k_, k_, k_, k_, k_
    };
    for (unsigned char i = 0; i < 10; i++)
    {
        Segmen::buffer[i + 4] = lookupSholat[((sholat - 2) * 10) + i];
        // layer[i + 4] = ON;
        // pgm_read_byte_near(lookupSholat + ((sholat - 2) * 8) + i);
    }
}

void Segmen::setTanggal(unsigned char date, unsigned char mount, int year)
{
    Segmen::buffer[4] = (Segmen::dataKalender[bagiSepuluh(date)]);
    Segmen::buffer[5] = (Segmen::dataKalender[sisaBagiSepuluh(date)]);
    Segmen::buffer[6] = 255 - 128;
    Segmen::buffer[7] = (Segmen::dataKalender[bagiSepuluh(mount)]);
    Segmen::buffer[8] = (Segmen::dataKalender[sisaBagiSepuluh(mount)]);
    Segmen::buffer[9] = 255 - 128;
    Segmen::buffer[10] = (Segmen::dataKalender[2]);
    Segmen::buffer[11] = (Segmen::dataKalender[0]);
    Segmen::buffer[12] = (Segmen::dataKalender[bagiSepuluh(year - 2000)]);
    Segmen::buffer[13] = (Segmen::dataKalender[sisaBagiSepuluh(year - 2000)]);
}

void Segmen::setImsya(unsigned char jam, unsigned char menit)
{
    Segmen::buffer[14] = (Segmen::dataJadwal[bagiSepuluh(jam)]);
    Segmen::buffer[15] = (Segmen::dataJadwal[sisaBagiSepuluh(jam)]);
    buffer[15] &= 0b11111110;
    Segmen::buffer[16] = (Segmen::dataJadwal[bagiSepuluh(menit)]);
    Segmen::buffer[17] = (Segmen::dataJadwal[sisaBagiSepuluh(menit)]);
}
void Segmen::setSubuh(unsigned char jam, unsigned char menit)
{
    Segmen::buffer[18] = (Segmen::dataJadwal[bagiSepuluh(jam)]);
    Segmen::buffer[19] = (Segmen::dataJadwal[sisaBagiSepuluh(jam)]);
    buffer[19] &= 0b11111110;
    Segmen::buffer[20] = (Segmen::dataJadwal[bagiSepuluh(menit)]);
    Segmen::buffer[21] = (Segmen::dataJadwal[sisaBagiSepuluh(menit)]);
}

void Segmen::setDzuhur(unsigned char jam, unsigned char menit)
{
    Segmen::buffer[22] = (Segmen::dataJadwal[bagiSepuluh(jam)]);
    Segmen::buffer[23] = (Segmen::dataJadwal[sisaBagiSepuluh(jam)]);
    buffer[23] &= 0b11111110;
    Segmen::buffer[24] = (Segmen::dataJadwal[bagiSepuluh(menit)]);
    Segmen::buffer[25] = (Segmen::dataJadwal[sisaBagiSepuluh(menit)]);
}
void Segmen::setAshar(unsigned char jam, unsigned char menit)
{
    Segmen::buffer[26] = (Segmen::dataJadwal[bagiSepuluh(jam)]);
    Segmen::buffer[27] = (Segmen::dataJadwal[sisaBagiSepuluh(jam)]);
    buffer[27] &= 0b11111110;
    Segmen::buffer[28] = (Segmen::dataJadwal[bagiSepuluh(menit)]);
    Segmen::buffer[29] = (Segmen::dataJadwal[sisaBagiSepuluh(menit)]);
}
void Segmen::setMaghrib(unsigned char jam, unsigned char menit)
{
    Segmen::buffer[30] = (Segmen::dataJadwal[bagiSepuluh(jam)]);
    Segmen::buffer[31] = (Segmen::dataJadwal[sisaBagiSepuluh(jam)]);
    buffer[31] &= 0b11111110;
    Segmen::buffer[32] = (Segmen::dataJadwal[bagiSepuluh(menit)]);
    Segmen::buffer[33] = (Segmen::dataJadwal[sisaBagiSepuluh(menit)]);
}

void Segmen::setIsya(unsigned char jam, unsigned char menit)
{
    Segmen::buffer[34] = Segmen::dataJadwal[bagiSepuluh(jam)];
    Segmen::buffer[35] = Segmen::dataJadwal[sisaBagiSepuluh(jam)];
    buffer[35] &= 0b11111110;
    Segmen::buffer[36] = Segmen::dataJadwal[bagiSepuluh(menit)];
    Segmen::buffer[37] = Segmen::dataJadwal[sisaBagiSepuluh(menit)];
}
void Segmen::setSuruq(unsigned char jam, unsigned char menit)
{
    Segmen::buffer[38] = (Segmen::dataJadwal[bagiSepuluh(jam)]);
    Segmen::buffer[39] = (Segmen::dataJadwal[sisaBagiSepuluh(jam)]);
    buffer[39] &= 0b11111110;
    Segmen::buffer[40] = (Segmen::dataJadwal[bagiSepuluh(menit)]);
    Segmen::buffer[41] = (Segmen::dataJadwal[sisaBagiSepuluh(menit)]);
}

void Segmen::displayHari(unsigned char hari)
{
    const unsigned char lookupKalender[70] = {
        k_, k_, ks, ke, kn, ki, kn, k_, k_, k_,
        k_, k_, ks, ke, kl, ka, ks, ka, k_, k_,
        k_, k_, k_, kr, ka, kb, ku, k_, k_, k_,
        k_, k_, kk, ka, km1, km2, ki, ks, k_, k_,
        k_, k_, kj, ku, km1, km2, ka, kt, k_, k_,
        k_, k_, ks, ka, kb, kt, ku, k_, k_, k_,
        k_, k_, k_, ka, kh, ka, kd, k_, k_, k_};
    for (unsigned char i = 0; i < 10; i++)
    {
        Segmen::buffer[i + 4] = lookupKalender[((hari - 1) * 10) + i];
    }
}
void Segmen::displayTogleOff()
{
    if (toggle)
    {
        // this->displayOff();
        this->displayNormal();
        toggle = false;
    }
    else
    {
        toggle = true;
        switch (Segmen::alarm)
        {
        case Segmen::modeImsya:
            this->displayImsya();
            break;
        case Segmen::modeSubuh:
            this->displaySubuh();
            break;
        case Segmen::modeSuruq:
            this->displaySuruq();
            break;
        case Segmen::modeDzuhur:
            this->displayDzuhur();
            break;
        case Segmen::modeJumat:
            this->displayJumat();
            break;
        case Segmen::modeAshar:
            this->displayAshar();
            break;
        case Segmen::modeMaghrib:
            this->displayMaghrib();
            break;
        case Segmen::modeIsya:
            this->displayIsya();
            break;
        case Segmen::modeIqomah:
            this->displayIqomah();
            break;
        default:
            break;
        }
    }
}

void Segmen::displayNormal()
{
    // Segmen::alarm = Segmen::modeNormal;
    for (unsigned char i = 4; i < 42; i++)
    {
        layer[i] = ON;
    }
}
void Segmen::displayIqomah()
{
    Segmen::alarm = Segmen::modeIqomah;
    for (unsigned char i = 4; i < 42; i++)
    {
        if (i < 14)
        {
            layer[i] = ON;
        }
        else
        {
            layer[i] = OFF;
        }
    }
    Segmen::displaySholat(Segmen::modeIqomah);
}
void Segmen::displayImsya()
{
    Segmen::alarm = Segmen::modeImsya;
    for (unsigned char i = 14; i < 18; i++)
    {
        layer[i] = OFF;
    }
    for (unsigned char i = 4; i < 14; i++)
    {
        layer[i] = OFF;
    }
    Segmen::displaySholat(Segmen::modeImsya);
    this->loop();
}
void Segmen::displaySubuh()
{
    Segmen::alarm = Segmen::modeSubuh;
    for (unsigned char i = 18; i < 22; i++)
    {
        layer[i] = OFF;
    }
    for (unsigned char i = 4; i < 14; i++)
    {
        layer[i] = OFF;
    }
    Segmen::displaySholat(Segmen::modeSubuh);
    this->loop();
}

void Segmen::displayJumat()
{
    Segmen::alarm = Segmen::modeJumat;

    for (unsigned char i = 22; i < 26; i++)
    {
        layer[i] = OFF;
    }

    for (unsigned char i = 4; i < 14; i++)
    {
        layer[i] = OFF;
    }
    Segmen::displaySholat(Segmen::modeJumat);
    this->loop();
}
void Segmen::displayDzuhur()
{
    Segmen::alarm = Segmen::modeDzuhur;

    for (unsigned char i = 22; i < 26; i++)
    {
        layer[i] = OFF;
    }

    for (unsigned char i = 4; i < 14; i++)
    {
        layer[i] = OFF;
    }
    Segmen::displaySholat(Segmen::modeDzuhur);
    this->loop();
}
void Segmen::displayAshar()
{
    Segmen::alarm = Segmen::modeAshar;
    for (unsigned char i = 26; i < 30; i++)
    {
        layer[i] = OFF;
    }

    for (unsigned char i = 4; i < 14; i++)
    {
        layer[i] = OFF;
    }
    Segmen::displaySholat(Segmen::modeAshar);
    this->loop();
}
void Segmen::displayMaghrib()
{
    Segmen::alarm = Segmen::modeMaghrib;
    for (unsigned char i = 30; i < 34; i++)
    {
        layer[i] = OFF;
    }

    for (unsigned char i = 4; i < 14; i++)
    {
        layer[i] = OFF;
    }
    Segmen::displaySholat(Segmen::modeMaghrib);
    this->loop();
}
void Segmen::displayIsya()
{
    Segmen::alarm = Segmen::modeIsya;

    for (unsigned char i = 34; i < 38; i++)
    {
        layer[i] = OFF;
    }

    for (unsigned char i = 4; i < 14; i++)
    {
        layer[i] = OFF;
    }
    Segmen::displaySholat(Segmen::modeIsya);
    this->loop();
}
void Segmen::displaySuruq()
{
    Segmen::alarm = Segmen::modeSuruq;

    for (unsigned char i = 38; i < 42; i++)
    {
        layer[i] = OFF;
    }

    for (unsigned char i = 4; i < 14; i++)
    {
        layer[i] = OFF;
    }
    Segmen::displaySholat(Segmen::modeSuruq);
    this->loop();
}
void Segmen::displayJamOff()
{
    Segmen::alarm = Segmen::modeJamOff;
    layer[0] = OFF;
    layer[1] = OFF;
}
void Segmen::displayMenitOff()
{
    Segmen::alarm = Segmen::modeMenitOff;
    layer[2] = OFF;
    layer[3] = OFF;
}
void Segmen::displayTanggalOff()
{
    Segmen::alarm = Segmen::modeTanggalOff;
    layer[4] = OFF;
    layer[5] = OFF;
}
void Segmen::displayBualnOff()
{
    layer[6] = OFF;
    layer[7] = OFF;
    Segmen::alarm = Segmen::modeBualnOff;
}
void Segmen::displayTahunOff()
{
    layer[8] = OFF;
    layer[9] = OFF;
    layer[10] = OFF;
    layer[11] = OFF;
    Segmen::alarm = Segmen::modeTahunOff;
}
void Segmen::displayOff()
{
    // modeOff;
    // Segmen::alarm = Segmen::modeOff;
    for (unsigned char i = 4; i < 40; i++)
    {
        layer[i] = OFF;
    }
}
void Segmen::shiftOut(uint8_t value)
{
    for (uint8_t b = 0; b < 8; b++)
    {
        if ((value & 0x80) == 0x80)
        {
            set_data()
        }
        else
        {
            clear_data();
        }
        value = (value << 1) | (value >> 7);
        clock_data();
    }
}
void Segmen::loop()
{
    int tick = millis() % 1000;
    tick < 500 ? sqw = true : sqw = false;
    if (sqw == true)
    {
        buffer[2] = buffer[2] & 0xf7;
    }
    else
    {
        buffer[2] = buffer[2] | 0x08;
    }
    clear_strobe();
    for (unsigned char i = 41; i < 42; i--)
    {
        if (i > 3)
        {
            this->shiftOut((buffer[i] | layer[i]));
        }
        else
        {
            this->shiftOut((buffer[i]));
        }
    }
    set_strobe();
}

// // 1,8 inchi
// if (sqw == HIGH)
//     segmen[2] = segmen[2] & 0xf7;
// else
//     segmen[2] = segmen[2] | 0x08;
// // 2,3 inchi
// if (sqw == HIGH)
//     segmen[1] = segmen[1] & 0xf7;
// else
//     segmen[1] = segmen[1] | 0x08;