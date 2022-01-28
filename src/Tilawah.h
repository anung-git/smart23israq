#if !defined(TILOAWAH_H)
#define TILOAWAH_H
#include <EEPROM.h>
#include "DFRobotDFPlayerMini.h"

struct EpromTilawah
{
    unsigned char volume;
    unsigned char timeSubuh;
    unsigned char timeDzuhur;
    unsigned char timeAshar;
    unsigned char timeMaghrib;
    unsigned char timeIsya;
    int indexSubuh;
    int indexDzuhur;
    int indexAshar;
    int indexMaghrib;
    int indexIsya;
};
class Tilawah
{
private:
    EpromTilawah eprom;
    DFRobotDFPlayerMini *player;

public:
    /*
    Folder diskripsi
    000 - 050 -> Adzan
    050 - 200 -> Alquran
    200 - 300 -> Sholawat
    */
    enum Folder
    {
        ADZAN,
        ALQURAN = 50,
        TARHIM_SHOLAWAT = 200
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

    void setVolume(unsigned char volume);

    void setPlaySubuh(unsigned char menit, int index);
    void setPlayDzuhur(unsigned char menit, int index);
    void setPlayAshar(unsigned char menit, int index);
    void setPlayMaghrib(unsigned char menit, int index);
    void setPlayIsya(unsigned char menit, int index);
    void setPlayImsya(unsigned char menit, int index);

    void setTime(unsigned char jam, unsigned char menit);
    void playQuran(unsigned char file);
    void playAdzan(unsigned char file);
    void playSholawat(unsigned char file);
    void stop();
    void loop();
};
#endif // TILOAWAH_H