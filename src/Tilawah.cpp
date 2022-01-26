#include "Tilawah.h"

Tilawah::Tilawah(/* args */)
{
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
