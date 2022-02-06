#if !defined(TILOAWAH_H)
#define TILOAWAH_H
#include "DFRobotDFPlayerMini.h"
#include "Memory.h"

class Tilawah
{
private:
    DFRobotDFPlayerMini *player;
    Memory *eprom;
    int timeSholat[6];
    unsigned char hari;

public:
    /*
    Folder diskripsi
    01 -> Adzan
    02 -> Alquran + Sholawat
    */

    Tilawah();
    ~Tilawah();
    void setPlayer(DFRobotDFPlayerMini *player);
    void setEprom(Memory *e);
    void setTimeSubuh(unsigned char jam, unsigned char menit);
    void setTimeDzuhur(unsigned char jam, unsigned char menit);
    void setTimeAshar(unsigned char jam, unsigned char menit);
    void setTimeMaghrib(unsigned char jam, unsigned char menit);
    void setTimeIsya(unsigned char jam, unsigned char menit);

    void setTime(unsigned char jam, unsigned char menit);
    void setHari(unsigned char hari);
    void playAdzanSubuh();
    void playAdzanDzuhur();
    void playAdzanAshar();
    void playAdzanMaghrib();
    void playAdzanIsya();
    void playAdzanJumat();
};
#endif // TILOAWAH_H