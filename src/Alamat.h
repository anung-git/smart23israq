#ifndef Alamat_H_
#define Alamat_H_
#define rumus

class Alamat
{
private:
public:
        enum ADDRES
        {
                ALARM_OFF,
                ALARM_IMSYA,
                ALARM_SUBUH,
                ALARM_SURUQ,
                ALARM_DZUHUR,
                ALARM_ASHAR,
                ALARM_MAGHRIB,
                ALARM_ISYA,
                ALARM_JUMAT,

#if defined(rumus)
                IQOMAH_SUBUH = 20,
#endif // rumus
#if !defined(rumus)
#define rumus
                IQOMAH_SUBUH = 7500,
#endif // rumus

                IQOMAH_DZUHUR,
                IQOMAH_ASHAR,
                IQOMAH_MAGHRIB,
                IQOMAH_ISYA,
                IQOMAH_JUMAT,

                ADZAN_SUBUH,
                ADZAN_DZUHUR,
                ADZAN_ASHAR,
                ADZAN_MAGHRIB,
                ADZAN_ISYA,
                ADZAN_JUMAT,

                SHOLAT_SUBUH,
                SHOLAT_DZUHUR,
                SHOLAT_ASHAR,
                SHOLAT_MAGHRIB,
                SHOLAT_ISYA,
                SHOLAT_JUMAT,

                OFFSITE_SUBUH,
                OFFSITE_DZUHUR,
                OFFSITE_ASHAR,
                OFFSITE_MAGHRIB,
                OFFSITE_ISYA,

        };
};

#endif