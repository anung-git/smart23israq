#include "Tilawah.h"

Tilawah::Tilawah(/* args */)
{
    EEPROM.get(1500, eprom);
}
void Tilawah::stop()
{
    this->player->stop();
}

void Tilawah::playQuran(unsigned char file)
{
    this->player->playFolder(ALQURAN, file);
}
void Tilawah::playAdzan(unsigned char file)
{
    this->player->playFolder(ADZAN, file);
}
void Tilawah::playSholawat(unsigned char file)
{
    this->player->playFolder(TARHIM_SHOLAWAT, file);
}
void Tilawah::setPlayer(DFRobotDFPlayerMini *player)
{
    this->player = player;
}
// volume 0...30
void Tilawah::setVolume(unsigned char volume)
{
    this->player->volume(volume);
}
void Tilawah::setTimeSubuh(unsigned char jam, unsigned char menit)
{
}
void Tilawah::setTimeDzuhur(unsigned char jam, unsigned char menit)
{
}
void Tilawah::setTimeAshar(unsigned char jam, unsigned char menit)
{
}
void Tilawah::setTimeMaghrib(unsigned char jam, unsigned char menit)
{
}
void Tilawah::setTimeIsya(unsigned char jam, unsigned char menit)
{
}
void Tilawah::setTimeImsya(unsigned char jam, unsigned char menit)
{
}
void Tilawah::loop()
{
}

Tilawah::~Tilawah()
{
}

// set time rtc
void Tilawah::setTime(unsigned char jam, unsigned char menit)
{
    int time = (jam * 60) + menit;
    int compare; //
    if (eprom.timeSubuh > 0)
    {
        compare = (eprom.timeSubuh * 60) + eprom.timeSubuh;
        if (time == compare)
        {
            this->player->playMp3Folder(eprom.indexSubuh);
        }
        if (time == (compare - eprom.timeSubuh))
        {
            this->player->playMp3Folder(eprom.indexSubuh);
        }
    }
    if (eprom.timeDzuhur > 0)
    {
        compare = (eprom.timeDzuhur * 60) + eprom.timeDzuhur;
        if (time == compare)
        {
            this->player->playMp3Folder(eprom.indexDzuhur);
        }
        if (time == (compare - eprom.timeDzuhur))
        {
            this->player->playMp3Folder(eprom.indexDzuhur);
        }
    }
    if (eprom.timeAshar > 0)
    {
        compare = (eprom.timeAshar * 60) + eprom.timeAshar;
        if (time == compare)
        {
            this->player->playMp3Folder(eprom.indexAshar);
        }
        if (time == (compare - eprom.timeAshar))
        {
            this->player->playMp3Folder(eprom.indexAshar);
        }
    }
    if (eprom.timeMaghrib > 0)
    {
        compare = (eprom.timeMaghrib * 60) + eprom.timeMaghrib;
        if (time == compare)
        {
            this->player->playMp3Folder(eprom.indexMaghrib);
        }
        if (time == (compare - eprom.timeMaghrib))
        {
            this->player->playMp3Folder(eprom.indexMaghrib);
        }
    }
    if (eprom.indexIsya > 0)
    {
        compare = (eprom.timeIsya * 60) + eprom.timeIsya;
        if (time == compare)
        {
            this->player->playMp3Folder(eprom.indexIsya);
        }
        if (time == (compare - eprom.timeIsya))
        {
            this->player->playMp3Folder(eprom.indexIsya);
        }
    }
}
