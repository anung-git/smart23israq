#include "Memory.h"

int Memory::getAddresInt(int addres)
{
    return (addres * 2) + END_BYTE;
}
int Memory::getAddresFloat(int addres)
{
    return (addres * 4) + getAddresInt(END_INT);
}
unsigned char Memory::readByte(int address)
{
    return EEPROM.read(address);
}
int Memory::readInt(int address)
{
    address = getAddresInt(address);
    unsigned char byte1 = EEPROM.read(address);
    unsigned char byte2 = EEPROM.read(address + 1);
    return (byte1 << 8) + byte2;
}

float Memory::readFloat(int address)
{
    address = getAddresFloat(address);
    float value = 0.0;
    unsigned char *p = (unsigned char *)(void *)&value;
    for (int i = 0; i < sizeof(value); i++)
    {
        *p++ = EEPROM.read(address++);
    }
    return value;
}

void Memory::writeByte(int address, unsigned char data)
{
    EEPROM.write(address, data);
}

void Memory::writeInt(int address, int data)
{
    address = getAddresInt(address);
    EEPROM.write(address, data >> 8);
    EEPROM.write(address + 1, data & 0xFF);
}

void Memory::writeFloat(int address, float data)
{
    address = getAddresFloat(address);
    unsigned char *p = (unsigned char *)(void *)&data;
    for (int i = 0; i < sizeof(data); i++)
    {
        EEPROM.write(address++, *p++);
    }
}
Memory::Memory()
{
}

Memory::~Memory()
{
}
// Getter Setter
unsigned char Memory::getBeep()
{
    return readByte(BEEP);
}
void Memory::setBeep(unsigned char beep)
{
    writeByte(BEEP, beep);
}
unsigned char Memory::getTimerAdzanSubuh()
{
    return readByte(TIMER_ADZAN_SUBUH);
}
void Memory::setTimerAdzanSubuh(unsigned char timer)
{
    writeByte(TIMER_ADZAN_SUBUH, timer);
}
unsigned char Memory::getTimerAdzanDuhur()
{
    return readByte(TAMBAH_KURANG_DUHUR);
}
void Memory::setTimerAdzanDuhur(unsigned char timer)
{
    writeByte(TAMBAH_KURANG_DUHUR, timer);
}
unsigned char Memory::getTimerAdzanAshar()
{
    return readByte(TIMER_ADZAN_ASHAR);
}
void Memory::setTimerAdzanAshar(unsigned char timer)
{
    writeByte(TIMER_ADZAN_ASHAR, timer);
}
unsigned char Memory::getTimerAdzanMaghrib()
{
    return readByte(TIMER_ADZAN_MAGHRIB);
}
void Memory::setTimerAdzanMaghrib(unsigned char timer)
{
    writeByte(TIMER_ADZAN_MAGHRIB, timer);
}
unsigned char Memory::getTimerAdzanIsya()
{
    return readByte(TIMER_ADZAN_ISYA);
}
void Memory::setTimerAdzanIsya(unsigned char timer)
{
    writeByte(TIMER_ADZAN_ISYA, timer);
}
unsigned char Memory::getTimerAdzanJumat()
{
    return readByte(TIMER_ADZAN_JUMAT);
}
void Memory::setTimerAdzanJumat(unsigned char timer)
{
    writeByte(TIMER_ADZAN_JUMAT, timer);
}
unsigned char Memory::getIqomahSubuh()
{
    return readByte(IQOMAH_SUBUH);
}
void Memory::setIqomahSubuh(unsigned char iqomah)
{
    writeByte(IQOMAH_SUBUH, iqomah);
}
unsigned char Memory::getIqomahDuhur()
{
    return readByte(IQOMAH_DUHUR);
}
void Memory::setIqomahDuhur(unsigned char iqomah)
{
    writeByte(IQOMAH_DUHUR, iqomah);
}
unsigned char Memory::getIqomahAshar()
{
    return readByte(IQOMAH_ASHAR);
}
void Memory::setIqomahAshar(unsigned char iqomah)
{
    writeByte(IQOMAH_ASHAR, iqomah);
}
unsigned char Memory::getIqomahMaghrib()
{
    return readByte(IQOMAH_MAGHRIB);
}
void Memory::setIqomahMaghrib(unsigned char iqomah)
{
    writeByte(IQOMAH_MAGHRIB, iqomah);
}
unsigned char Memory::getIqomahIsya()
{
    return readByte(IQOMAH_ISYA);
}
void Memory::setIqomahIsya(unsigned char iqomah)
{
    writeByte(IQOMAH_ISYA, iqomah);
}
unsigned char Memory::getIqomahJumat()
{
    return readByte(IQOMAH_JUMAT);
}
void Memory::setIqomahJumat(unsigned char iqomah)
{
    writeByte(IQOMAH_JUMAT, iqomah);
}
unsigned char Memory::getLamaSholatSubuh()
{
    return readByte(LAMA_SHOLAT_SUBUH);
}
void Memory::setLamaSholatSubuh(unsigned char lamaSholat)
{
    writeByte(LAMA_SHOLAT_SUBUH, lamaSholat);
}

unsigned char Memory::getLamaSholatDuhur()
{
    return readByte(LAMA_SHOLAT_DUHUR);
}
void Memory::setLamaSholatDuhur(unsigned char lamaSholat)
{
    writeByte(LAMA_SHOLAT_DUHUR, lamaSholat);
}
unsigned char Memory::getLamaSholatAshar()
{
    return readByte(LAMA_SHOLAT_ASHAR);
}
void Memory::setLamaSholatAshar(unsigned char lamaSholat)
{
    writeByte(LAMA_SHOLAT_ASHAR, lamaSholat);
}
unsigned char Memory::getLamaSholatMaghrib()
{
    return readByte(LAMA_SHOLAT_MAGHRIB);
}
void Memory::setLamaSholatMaghrib(unsigned char lamaSholat)
{
    writeByte(LAMA_SHOLAT_MAGHRIB, lamaSholat);
}
unsigned char Memory::getLamaSholatIsya()
{
    return readByte(LAMA_SHOLAT_ISYA);
}
void Memory::setLamaSholatIsya(unsigned char lamaSholat)
{
    writeByte(LAMA_SHOLAT_ISYA, lamaSholat);
}
unsigned char Memory::getLamaSholatJumat()
{
    return readByte(LAMA_SHOLAT_JUMAT);
}
void Memory::setLamaSholatJumat(unsigned char lamaSholat)
{
    writeByte(LAMA_SHOLAT_JUMAT, lamaSholat);
}
unsigned char Memory::getVolumemp3()
{
    return readByte(VOLUME_MP3);
}
void Memory::setVolumemp3(unsigned char volumemp3)
{
    // disable intterup
    writeByte(VOLUME_MP3, volumemp3);
}
unsigned char Memory::getEqualizer()
{
    return readByte(EQUALIZER);
}
void Memory::setEqualizer(unsigned char equalizer)
{
    writeByte(EQUALIZER, equalizer);
}
unsigned char Memory::getTimeTilawahSubuh()
{
    return readByte(TIME_TILAWAH_SUBUH);
}
void Memory::setTimeTilawahSubuh(unsigned char timeTilawah)
{
    writeByte(TIME_TILAWAH_SUBUH, timeTilawah);
}
unsigned char Memory::getTimeTilawahDzuhur()
{
    return readByte(TIME_TILAWAH_DZUHUR);
}
void Memory::setTimeTilawahDzuhur(unsigned char timeTilawah)
{
    writeByte(TIME_TILAWAH_DZUHUR, timeTilawah);
}
unsigned char Memory::getTimeTilawahAshar()
{
    return readByte(TIME_TILAWAH_ASHAR);
}
void Memory::setTimeTilawahAshar(unsigned char timeTilawah)
{
    writeByte(TIME_TILAWAH_ASHAR, timeTilawah);
}
unsigned char Memory::getTimeTilawahMaghrib()
{
    return readByte(TIME_TILAWAH_MAGHRIB);
}
void Memory::setTimeTilawahMaghrib(unsigned char timeTilawah)
{
    writeByte(TIME_TILAWAH_MAGHRIB, timeTilawah);
}
unsigned char Memory::getTimeTilawahIsya()
{
    return readByte(TIME_TILAWAH_ISYA);
}
void Memory::setTimeTilawahIsya(unsigned char timeTilawah)
{
    writeByte(TIME_TILAWAH_ISYA, timeTilawah);
}
unsigned char Memory::getTimeTilawahJumat()
{
    return readByte(TIME_TILAWAH_JUMAT);
}
void Memory::setTimeTilawahJumat(unsigned char timeTilawah)
{
    writeByte(TIME_TILAWAH_JUMAT, timeTilawah);
}
int Memory::getJadwalFixSubuh()
{
    return readInt(JADWAL_FIX_SUBUH);
}
void Memory::setJadwalFixSubuh(int jadwalFix)
{
    writeInt(JADWAL_FIX_SUBUH, jadwalFix);
}
int Memory::getJadwalFixDzuhur()
{
    return readInt(JADWAL_FIX_DUHUR);
}
void Memory::setJadwalFixDzuhur(int jadwalFix)
{
    writeInt(JADWAL_FIX_DUHUR, jadwalFix);
}
int Memory::getJadwalFixAshar()
{
    return readInt(JADWAL_FIX_ASHAR);
}
void Memory::setJadwalFixAshar(int jadwalFix)
{
    writeInt(JADWAL_FIX_ASHAR, jadwalFix);
}
int Memory::getJadwalFixMaghrib()
{
    return readInt(JADWAL_FIX_MAGHRIB);
}
void Memory::setJadwalFixMaghrib(int jadwalFix)
{
    writeInt(JADWAL_FIX_MAGHRIB, jadwalFix);
}
int Memory::getJadwalFixIsya()
{
    return readInt(JADWAL_FIX_ISYA);
}
void Memory::setJadwalFixIsya(int jadwalFix)
{
    writeInt(JADWAL_FIX_ISYA, jadwalFix);
}

int Memory::getIndexTilawahSubuh()
{
    return readInt(INDEX_TILAWAH_SUBUH);
}
void Memory::setIndexTilawahSubuh(int index)
{
    writeInt(INDEX_TILAWAH_SUBUH, index);
}
int Memory::getIndexTilawahDzuhur()
{
    return readInt(INDEX_TILAWAH_DZUHUR);
}
void Memory::setIndexTilawahDzuhur(int index)
{
    writeInt(INDEX_TILAWAH_DZUHUR, index);
}
int Memory::getIndexTilawahAshar()
{
    return readInt(INDEX_TILAWAH_ASHAR);
}
void Memory::setIndexTilawahAshar(int index)
{
    writeInt(INDEX_TILAWAH_ASHAR, index);
}

int Memory::getIndexTilawahMaghrib()
{
    return readInt(INDEX_TILAWAH_MAGHRIB);
}
void Memory::setIndexTilawahMaghrib(int index)
{
    writeInt(INDEX_TILAWAH_MAGHRIB, index);
}
int Memory::getIndexTilawahIsya()
{
    return readInt(INDEX_TILAWAH_ISYA);
}
void Memory::setIndexTilawahIsya(int index)
{
    writeInt(INDEX_TILAWAH_ISYA, index);
}
int Memory::getIndexTilawahJumat()
{
    return readInt(INDEX_TILAWAH_JUMAT);
}
void Memory::setIndexTilawahJumat(int index)
{
    writeInt(INDEX_TILAWAH_JUMAT, index);
}

unsigned char Memory::getIndexAdzanSubuh()
{
    return readByte(INDEX_ADZAN_SUBUH);
}
void Memory::setIndexAdzanSubuh(unsigned char index)
{
    writeByte(INDEX_ADZAN_SUBUH, index);
}
unsigned char Memory::getIndexAdzanDzuhur()
{
    return readByte(INDEX_ADZAN_DZUHUR);
}
void Memory::setIndexAdzanDzuhur(unsigned char index)
{
    writeByte(INDEX_ADZAN_DZUHUR, index);
}
unsigned char Memory::getIndexAdzanAshar()
{
    return readByte(INDEX_ADZAN_ASHAR);
}
void Memory::setIndexAdzanAshar(unsigned char index)
{
    writeByte(INDEX_ADZAN_ASHAR, index);
}
unsigned char Memory::getIndexAdzanMaghrib()
{
    return readByte(INDEX_ADZAN_MAGHRIB);
}
void Memory::setIndexAdzanMaghrib(unsigned char index)
{
    writeByte(INDEX_ADZAN_MAGHRIB, index);
}
unsigned char Memory::getIndexAdzanIsya()
{
    return readByte(INDEX_ADZAN_ISYA);
}
void Memory::setIndexAdzanIsya(unsigned char index)
{
    writeByte(INDEX_ADZAN_ISYA, index);
}
unsigned char Memory::getIndexAdzanJumat()
{
    return readByte(INDEX_ADZAN_JUMAT);
}
void Memory::setIndexAdzanJumat(unsigned char index)
{
    writeByte(INDEX_ADZAN_JUMAT, index);
}
float Memory::getKotaLintang()
{
    return readFloat(KOTA_LINTANG);
}
void Memory::setKotaLintang(float kotaLintang)
{
    writeFloat(KOTA_LINTANG, kotaLintang);
}
float Memory::getKotaBujur()
{
    return readFloat(KOTA_BUJUR);
}
void Memory::setKotaBujur(float kotaBujur)
{
    writeFloat(KOTA_BUJUR, kotaBujur);
}
void Memory::setKota(int kota)
{
    writeInt(KOTA, kota);
}
int Memory::getTimeOn()
{
    return readInt(KOTA);
}
int Memory::getTimeOff()
{
    return readInt(TIME_OFF);
}
int Memory::getKota()
{
    return readInt(KOTA);
}
void Memory::setTimeOn(int time)
{
    writeInt(TIME_ON, time);
}
void Memory::setTimeOff(int time)
{
    writeInt(TIME_OFF, time);
}
int Memory::getTambahKurangSubuh()
{
    return readInt(TAMBAH_KURANG_SUBUH);
}
void Memory::setTambahKurangSubuh(int offsite)
{
    writeInt(TAMBAH_KURANG_SUBUH, offsite);
}
int Memory::getTambahKurangDuhur()
{
    return readInt(TAMBAH_KURANG_DUHUR);
}
void Memory::setTambahKurangDuhur(int offsite)
{
    writeInt(TAMBAH_KURANG_DUHUR, offsite);
}
int Memory::getTambahKurangAshar()
{
    return readInt(TAMBAH_KURANG_ASHAR);
}
void Memory::setTambahKurangAshar(int offsite)
{
    writeInt(TAMBAH_KURANG_ASHAR, offsite);
}
int Memory::getTambahKurangMaghrib()
{
    return readInt(TAMBAH_KURANG_MAGHRIB);
}
void Memory::setTambahKurangMaghrib(int offsite)
{
    writeInt(TAMBAH_KURANG_MAGHRIB, offsite);
}
int Memory::getTambahKurangIsya()
{
    return readInt(TAMBAH_KURANG_ISYA);
}
void Memory::setTambahKurangIsya(int offsite)
{
    writeInt(TAMBAH_KURANG_ISYA, offsite);
}
unsigned char Memory::getKotaGMT()
{
    return readByte(SET_KOTA_GMT);
}
void Memory::setKotaGMT(unsigned char gmt)
{
    writeByte(SET_KOTA_GMT, gmt);
}
// Memory::getKota() {}
// template <class T> int EEPROM_writeAnything(int ee, const T& value)
// {
//     const byte* p = (const byte*)(const void*)&value;
//     int i;
//     for (i = 0; i < sizeof(value); i++)
//         EEPROM.write(ee++, *p++);
//     return i;
// }

// template <class T> int EEPROM_readAnything(int ee, T& value)
// {
//     byte* p = (byte*)(void*)&value;
//     int i;
//     for (i = 0; i < sizeof(value); i++)
//         *p++ = EEPROM.read(ee++);
//     return i;
// }