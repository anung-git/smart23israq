#include "Tilawah.h"

Tilawah::Tilawah(/* args */)
{
}

void Tilawah::playAdzanSubuh()
{
    unsigned char track = eprom->getIndexAdzanSubuh();
    if (track)
    {
        player->playFolder(1, track);
    }
}
void Tilawah::playAdzanDzuhur()
{
    unsigned char track = eprom->getIndexAdzanDzuhur();
    if (track)
    {
        player->playFolder(1, track);
    }
}
void Tilawah::playAdzanAshar()
{
    unsigned char track = eprom->getIndexAdzanAshar();
    if (track)
    {
        player->playFolder(1, track);
    }
}
void Tilawah::playAdzanMaghrib()
{
    unsigned char track = eprom->getIndexAdzanMaghrib();
    if (track)
    {
        player->playFolder(1, track);
    }
}
void Tilawah::playAdzanIsya()
{
    unsigned char track = eprom->getIndexAdzanIsya();
    if (track)
    {
        player->playFolder(1, track);
    }
}
void Tilawah::playAdzanJumat()
{
    unsigned char track = eprom->getIndexAdzanJumat();
    if (track)
    {
        player->playFolder(1, track);
    }
}

void Tilawah::setPlayer(DFRobotDFPlayerMini *player)
{
    this->player = player;
}
void Tilawah::setEprom(Memory *e)
{
    this->eprom = e;
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

Tilawah::~Tilawah()
{
}
void Tilawah::setHari(unsigned char hari)
{
    this->hari = hari;
}

// set time rtc
void Tilawah::setTime(unsigned char jam, unsigned char menit)
{
    int time = (jam * 60) + menit;
    int timeTilawahSubuh = timeSholat[0] - eprom->getTimeTilawahSubuh();
    int timeTilawahDzuhur = timeSholat[1] - eprom->getTimeTilawahDzuhur();
    int timeTilawahJumat = timeSholat[1] - eprom->getTimeTilawahJumat();
    int timeTilawahAshar = timeSholat[2] - eprom->getTimeTilawahAshar();
    int timeTilawahMaghrib = timeSholat[3] - eprom->getTimeTilawahMaghrib();
    int timeTilawahIsya = timeSholat[4] - eprom->getTimeTilawahIsya();

    if (hari == 5) // jumatan
    {
        if (time == timeTilawahJumat)
        {
            unsigned char track = eprom->getIndexTilawahJumat();
            if (track != 0)
            {
                player->playFolder(2, track);
            }
        }
    }
    else // dzuhur
    {
        if (time == timeTilawahDzuhur)
        {
            unsigned char track = eprom->getIndexTilawahDzuhur();
            if (track != 0)
            {
                player->playFolder(2, track);
            }
        }
    }

    if (time == timeTilawahSubuh)
    {
        unsigned char track = eprom->getIndexTilawahSubuh();
        if (track != 0)
        {
            player->playFolder(2, track);
        }
    }

    else if (time == timeTilawahAshar)
    {
        unsigned char track = eprom->getIndexTilawahAshar();
        if (track != 0)
        {
            player->playFolder(2, track);
        }
    }
    else if (time == timeTilawahMaghrib)
    {
        unsigned char track = eprom->getIndexTilawahMaghrib();
        if (track != 0)
        {
            player->playFolder(2, track);
        }
    }
    else if (time == timeTilawahIsya)
    {
        unsigned char track = eprom->getIndexTilawahIsya();
        if (track != 0)
        {
            player->playFolder(2, track);
        }
    }
}

// create task scheduler
//  void Tilawah::createTaskScheduler()
//  {
//      // create task scheduler
//      scheduler.addTask(this->loop, 1000, true);
//  }