#ifndef Alamat_H_
#define Alamat_H_
#define rumus

class Alamat
{
private:
public:
        enum RUNNING_TEXT
        {
                TEXT_IQ_SUBUH = 100,
                TEXT_IQ_DUHUR = 200,
                TEXT_IQ_ASHAR = 300,
                TEXT_IQ_MAGHRIB = 400,
                TEXT_IQ_ISYA = 500,
                TEXT_IQ_JUMAT = 600,
                TEXT_RUN = 700
        };
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

                // IQOMAH_SUBUH = 20,
                // IQOMAH_DZUHUR,
                // IQOMAH_ASHAR,
                // IQOMAH_MAGHRIB,
                // IQOMAH_ISYA,
                // IQOMAH_JUMAT,

                // ADZAN_SUBUH,
                // ADZAN_DZUHUR,
                // ADZAN_ASHAR,
                // ADZAN_MAGHRIB,
                // ADZAN_ISYA,
                // ADZAN_JUMAT,

                // SHOLAT_SUBUH,
                // SHOLAT_DZUHUR,
                // SHOLAT_ASHAR,
                // SHOLAT_MAGHRIB,
                // SHOLAT_ISYA,
                // SHOLAT_JUMAT,

                // OFFSITE_SUBUH,
                // OFFSITE_DZUHUR,
                // OFFSITE_ASHAR,
                // OFFSITE_MAGHRIB,
                // OFFSITE_ISYA,
        };
};

#endif