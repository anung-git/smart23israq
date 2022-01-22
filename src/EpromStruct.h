#if !defined(EPROMSTRUCT_H)
#define EPROMSTRUCT_H

struct EpromObject
{
    int nullSafety;
    int kota;
    unsigned char beep;
    unsigned char timer_adzan_subuh;
    unsigned char timer_adzan_duhur;
    unsigned char timer_adzan_ashar;
    unsigned char timer_adzan_maghrib;
    unsigned char timer_adzan_isya;
    unsigned char timer_adzan_jumat;

    unsigned char iqomah_subuh;
    unsigned char iqomah_duhur;
    unsigned char iqomah_ashar;
    unsigned char iqomah_maghrib;
    unsigned char iqomah_isya;
    unsigned char iqomah_jumat;

    unsigned char lama_sholat_subuh;
    unsigned char lama_sholat_duhur;
    unsigned char lama_sholat_ashar;
    unsigned char lama_sholat_maghrib;
    unsigned char lama_sholat_isya;
    unsigned char lama_sholat_jumat;
    unsigned char tartil_subuh; // 0= mati dan >0 hidup
    unsigned char tartil_duhur;
    unsigned char tartil_ashar;
    unsigned char tartil_maghrib;
    unsigned char tartil_isya;
    unsigned char tartil_jumat;
    unsigned char kecerahan_1;
    unsigned char kecerahan_2;
    unsigned char kecerahan_3;
    unsigned char kecerahan_4;
    int jam_kecerahan_1;
    int jam_kecerahan_2;
    int jam_kecerahan_3;
    int jam_kecerahan_4;
    int tambah_kurang_subuh;
    int tambah_kurang_duhur;
    int tambah_kurang_ashar;
    int tambah_kurang_maghrib;
    int tambah_kurang_isya;
    int jadwal_fix_subuh;
    int jadwal_fix_duhur;
    int jadwal_fix_ashar;
    int jadwal_fix_maghrib;
    int jadwal_fix_isya;
    unsigned char mazhab_ashar;
    int set_kota_gmt;
    float set_kota_lnt;
    float set_kota_bjr;
    int timeOn;
    int timeOff;
};

#endif // EPROMSTRUCT_H