#if !defined(MEMORY_h)
#define MEMORY_h
#include <EEPROM.h>
enum AddresseByte
{
    // DATA BYTE
    BEEP = 1,
    TIMER_ADZAN_SUBUH,
    TIMER_ADZAN_DUHUR,
    TIMER_ADZAN_ASHAR,
    TIMER_ADZAN_MAGHRIB,
    TIMER_ADZAN_ISYA,
    TIMER_ADZAN_JUMAT,

    IQOMAH_SUBUH,
    IQOMAH_DUHUR,
    IQOMAH_ASHAR,
    IQOMAH_MAGHRIB,
    IQOMAH_ISYA,
    IQOMAH_JUMAT,

    LAMA_SHOLAT_SUBUH,
    LAMA_SHOLAT_DUHUR,
    LAMA_SHOLAT_ASHAR,
    LAMA_SHOLAT_MAGHRIB,
    LAMA_SHOLAT_ISYA,
    LAMA_SHOLAT_JUMAT,

    VOLUME_MP3,
    EQUALIZER,
    TIME_TILAWAH_SUBUH,
    TIME_TILAWAH_DZUHUR,
    TIME_TILAWAH_ASHAR,
    TIME_TILAWAH_MAGHRIB,
    TIME_TILAWAH_ISYA,
    TIME_TILAWAH_JUMAT,
    SET_KOTA_GMT,

    INDEX_ADZAN_SUBUH,
    INDEX_ADZAN_DZUHUR,
    INDEX_ADZAN_ASHAR,
    INDEX_ADZAN_MAGHRIB,
    INDEX_ADZAN_ISYA,
    INDEX_ADZAN_JUMAT,
    END_BYTE,

};
enum AddresInt
{
    KOTA,
    TAMBAH_KURANG_SUBUH,
    TAMBAH_KURANG_DUHUR,
    TAMBAH_KURANG_ASHAR,
    TAMBAH_KURANG_MAGHRIB,
    TAMBAH_KURANG_ISYA,
    JADWAL_FIX_SUBUH,
    JADWAL_FIX_DUHUR,
    JADWAL_FIX_ASHAR,
    JADWAL_FIX_MAGHRIB,
    JADWAL_FIX_ISYA,
    TIME_ON,
    TIME_OFF,
    INDEX_TILAWAH_SUBUH,
    INDEX_TILAWAH_DZUHUR,
    INDEX_TILAWAH_ASHAR,
    INDEX_TILAWAH_MAGHRIB,
    INDEX_TILAWAH_ISYA,
    INDEX_TILAWAH_JUMAT,

    END_INT,
};
enum AddresFloat
{
    KOTA_LINTANG,
    KOTA_BUJUR

};
class Memory
{
private:
    unsigned char readByte(int address);
    int readInt(int address);
    float readFloat(int address);
    void writeByte(int address, unsigned char data);
    void writeInt(int address, int data);
    void writeFloat(int address, float data);
    int getAddresInt(int addres);
    int getAddresFloat(int addres);

public:
    Memory();
    ~Memory();
    unsigned char getBeep();
    void setBeep(unsigned char beep);
    
    unsigned char getTimerAdzanSubuh();
    void setTimerAdzanSubuh(unsigned char timer);
    unsigned char getTimerAdzanDuhur();
    void setTimerAdzanDuhur(unsigned char timer);
    unsigned char getTimerAdzanAshar();
    void setTimerAdzanAshar(unsigned char timer);
    unsigned char getTimerAdzanMaghrib();
    void setTimerAdzanMaghrib(unsigned char timer);
    unsigned char getTimerAdzanIsya();
    void setTimerAdzanIsya(unsigned char timer);
    unsigned char getTimerAdzanJumat();
    void setTimerAdzanJumat(unsigned char timer);

    unsigned char getIqomahSubuh();
    void setIqomahSubuh(unsigned char iqomah);
    unsigned char getIqomahDuhur();
    void setIqomahDuhur(unsigned char iqomah);
    unsigned char getIqomahAshar();
    void setIqomahAshar(unsigned char iqomah);
    unsigned char getIqomahMaghrib();
    void setIqomahMaghrib(unsigned char iqomah);
    unsigned char getIqomahIsya();
    void setIqomahIsya(unsigned char iqomah);
    unsigned char getIqomahJumat();
    void setIqomahJumat(unsigned char iqomah);

    unsigned char getLamaSholatSubuh();
    void setLamaSholatSubuh(unsigned char lamaSholat);
    unsigned char getLamaSholatDuhur();
    void setLamaSholatDuhur(unsigned char lamaSholat);
    unsigned char getLamaSholatAshar();
    void setLamaSholatAshar(unsigned char lamaSholat);
    unsigned char getLamaSholatMaghrib();
    void setLamaSholatMaghrib(unsigned char lamaSholat);
    unsigned char getLamaSholatIsya();
    void setLamaSholatIsya(unsigned char lamaSholat);
    unsigned char getLamaSholatJumat();
    void setLamaSholatJumat(unsigned char lamaSholat);

    unsigned char getVolumemp3();
    void setVolumemp3(unsigned char volumemp3);
    unsigned char getEqualizer();
    void setEqualizer(unsigned char equalizer);

    unsigned char getTimeTilawahSubuh();
    void setTimeTilawahSubuh(unsigned char timeTilawah);
    unsigned char getTimeTilawahDzuhur();
    void setTimeTilawahDzuhur(unsigned char timeTilawah);
    unsigned char getTimeTilawahAshar();
    void setTimeTilawahAshar(unsigned char timeTilawah);
    unsigned char getTimeTilawahMaghrib();
    void setTimeTilawahMaghrib(unsigned char timeTilawah);
    unsigned char getTimeTilawahIsya();
    void setTimeTilawahIsya(unsigned char timeTilawah);
    unsigned char getTimeTilawahJumat();
    void setTimeTilawahJumat(unsigned char timeTilawah);

    int getTambahKurangSubuh();
    void setTambahKurangSubuh(int tambahKurang);
    int getTambahKurangDuhur();
    void setTambahKurangDuhur(int tambahKurang);
    int getTambahKurangAshar();
    void setTambahKurangAshar(int tambahKurang);
    int getTambahKurangMaghrib();
    void setTambahKurangMaghrib(int tambahKurang);
    int getTambahKurangIsya();
    void setTambahKurangIsya(int tambahKurang);

    int getKota();
    void setKota(int kota);
    unsigned char getKotaGMT();
    void setKotaGMT(unsigned char gmt);
    int getTimeOn();
    void setTimeOn(int timeOn);
    int getTimeOff();
    void setTimeOff(int timeOff);

    int getJadwalFixSubuh();
    void setJadwalFixSubuh(int jadwalFix);
    int getJadwalFixDzuhur();
    void setJadwalFixDzuhur(int jadwalFix);
    int getJadwalFixAshar();
    void setJadwalFixAshar(int jadwalFix);
    int getJadwalFixMaghrib();
    void setJadwalFixMaghrib(int jadwalFix);
    int getJadwalFixIsya();
    void setJadwalFixIsya(int jadwalFix);

    int getIndexTilawahSubuh();
    void setIndexTilawahSubuh(int index);
    int getIndexTilawahDzuhur();
    void setIndexTilawahDzuhur(int index);
    int getIndexTilawahAshar();
    void setIndexTilawahAshar(int index);
    int getIndexTilawahMaghrib();
    void setIndexTilawahMaghrib(int index);
    int getIndexTilawahIsya();
    void setIndexTilawahIsya(int index);
    int getIndexTilawahJumat();
    void setIndexTilawahJumat(int index);

    unsigned char getIndexAdzanSubuh();
    void setIndexAdzanSubuh(unsigned char index);
    unsigned char getIndexAdzanDzuhur();
    void setIndexAdzanDzuhur(unsigned char index);
    unsigned char getIndexAdzanAshar();
    void setIndexAdzanAshar(unsigned char index);
    unsigned char getIndexAdzanMaghrib();
    void setIndexAdzanMaghrib(unsigned char index);
    unsigned char getIndexAdzanIsya();
    void setIndexAdzanIsya(unsigned char index);
    unsigned char getIndexAdzanJumat();
    void setIndexAdzanJumat(unsigned char index);

    float getKotaLintang();
    void setKotaLintang(float kotaLintang);
    float getKotaBujur();
    void setKotaBujur(float kotaBujur);
};

#endif // MEMORY_h
