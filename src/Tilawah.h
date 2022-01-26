#if !defined(TILOAWAH_H)
#define TILOAWAH_H

#include "DFRobotDFPlayerMini.h"
class Tilawah
{
private:
    DFRobotDFPlayerMini *player;
    /* data */
public:
    /*
    Folder diskripsi
    00 -> Adzan
    01 -> Alquran 30juz 114 surat
    02 -> Tarhim dan Sholawat
    */
    enum Folder
    {
        ADZAN,
        ALQURAN,
        TARHIM_SHOLAWAT
    };
    Tilawah(/* args */);
    ~Tilawah();
    void setPlayer(DFRobotDFPlayerMini *player);
    void setTimeSubuh(unsigned char jam, unsigned char menit);
    void setTimeDzuhur(unsigned char jam, unsigned char menit);
    void setTimeAshar(unsigned char jam, unsigned char menit);
    void setTimeMaghrib(unsigned char jam, unsigned char menit);
    void setTimeIsya(unsigned char jam, unsigned char menit);
    void setTimeImsya(unsigned char jam, unsigned char menit);
    void playQuran(unsigned char file);
    void playAdzan(unsigned char file);
    void playSholawat(unsigned char file);
    void stop();
    void loop();
};
#endif // TILOAWAH_H