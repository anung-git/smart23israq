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
    timeSholat[0] = jam * 60 + menit;
}
void Tilawah::setTimeDzuhur(unsigned char jam, unsigned char menit)
{
    timeSholat[1] = jam * 60 + menit;
}
void Tilawah::setTimeAshar(unsigned char jam, unsigned char menit)
{
    timeSholat[2] = jam * 60 + menit;
}
void Tilawah::setTimeMaghrib(unsigned char jam, unsigned char menit)
{
    timeSholat[3] = jam * 60 + menit;
}
void Tilawah::setTimeIsya(unsigned char jam, unsigned char menit)
{
    timeSholat[4] = jam * 60 + menit;
}
void Tilawah::setTimeImsya(unsigned char jam, unsigned char menit)
{
    timeSholat[5] = jam * 60 + menit;
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
    if (eprom.timeSubuh > 0)
    {
        if (time == timeSholat[0])
        {
            this->player->playMp3Folder(eprom.indexSubuh);
        }
        if (time == (timeSholat[0] - eprom.timeSubuh))
        {
            this->player->playMp3Folder(eprom.indexSubuh);
        }
    }
    if (eprom.timeDzuhur > 0)
    {
        if (time == timeSholat[1])
        {
            this->player->playMp3Folder(eprom.indexDzuhur);
        }
        if (time == (timeSholat[1] - eprom.timeDzuhur))
        {
            this->player->playMp3Folder(eprom.indexDzuhur);
        }
    }
    if (eprom.timeAshar > 0)
    {
        if (time == timeSholat[2])
        {
            this->player->playMp3Folder(eprom.indexAshar);
        }
        if (time == (timeSholat[2] - eprom.timeAshar))
        {
            this->player->playMp3Folder(eprom.indexAshar);
        }
    }
    if (eprom.timeMaghrib > 0)
    {

        if (time == timeSholat[3])
        {
            this->player->playMp3Folder(eprom.indexMaghrib);
        }
        if (time == (timeSholat[3] - eprom.timeMaghrib))
        {
            this->player->playMp3Folder(eprom.indexMaghrib);
        }
    }
    if (eprom.indexIsya > 0)
    {
        if (time == timeSholat[4])
        {
            this->player->playMp3Folder(eprom.indexIsya);
        }
        if (time == (timeSholat[4] - eprom.timeIsya))
        {
            this->player->playMp3Folder(eprom.indexIsya);
        }
    }
}

//create task scheduler
void Tilawah::createTaskScheduler()
{
    // create task scheduler
    scheduler.addTask(this->loop, 1000, true);
}