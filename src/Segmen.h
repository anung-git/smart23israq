#ifndef Segmen_H_
#define Segmen_H_

#define OFF 0xFF
#define ON 0x00

#define LAYER_JAM_OFF 0xFF
#define LAYER_JAM_ON 0x00

// #define ka 123
// #define kb 124
// #define kd 121
// #define ke 111
// #define kf 78
// #define kg 63
// #define kh 92
// #define ki 17
// #define kj 113
// #define kk 94
// #define kl 100
// #define km1 71
// #define km2 23
// #define kn 87
// #define ko 119
// #define kp 79
// #define kq 31
// #define kr 70
// #define ks 62
// #define kt 108
// #define ku 117
// #define ky 61
// #define k_ 0

#define ka 65
#define kb 49
#define kd 97
#define ke 3
#define kf 23
#define kg 9
#define kh 53
#define ki 237
#define kj 225
#define kk 21
#define kl 179
#define km1 135
#define km2 141
#define kn 133
#define ko 129
#define kp 7
#define kq 13
#define kr 151
#define ks 25
#define kt 51
#define ku 161
#define ky 41
#define kw1 177
#define kw2 225
#define k_ 255

#define set_data()     \
  {                    \
    PORTA |= _BV(PA5); \
  }
#define clear_data()    \
  {                     \
    PORTA &= ~_BV(PA5); \
  }

#define clock_data()    \
  {                     \
    PORTA |= _BV(PA6);  \
    PORTA &= ~_BV(PA6); \
  }
#define set_strobe()   \
  {                    \
    PORTA |= _BV(PA4); \
  }
#define clear_strobe()  \
  {                     \
    PORTA &= ~_BV(PA4); \
  }

#include <avr/pgmspace.h>

class Segmen
{
private:
  bool toggle, sqw;
  unsigned char alarm;
  // uint16_t compare;
  volatile unsigned char layer[42];
  volatile unsigned char buffer[42];
  // 2,3 inchi
  // const byte dataJam[] = {119, 20, 179, 182, 212, 230, 231, 52, 247, 246, 0};
  // 1,8 inchi
  // const byte dataJam[] = {136, 238, 73, 76, 46, 28, 24, 206, 8, 12, 255};
  const unsigned char dataJam[11] = {136, 238, 73, 76, 46, 28, 24, 206, 8, 12, 255};
  const unsigned char dataKalender[11] = {129, 237, 67, 73, 45, 25, 17, 205, 1, 9, 255};
  const unsigned char dataJadwal[11] = {129, 237, 67, 73, 45, 25, 17, 205, 1, 9, 255};

  unsigned char bagiSepuluh(unsigned char nilai);
  unsigned char sisaBagiSepuluh(unsigned char nilai);

  const unsigned char segment_data = 3;
  const unsigned char segment_clk = 2;
  const unsigned char segment_stb = 4;
  enum modeSegmen
  {
    modeNormal,
    modeOff,
    modeImsya,
    modeSubuh,
    modeSuruq,
    modeDzuhur,
    modeJumat,
    modeAshar,
    modeMaghrib,
    modeIsya,
    modeIqomah,
    modeKutbah,
    modeSetJam,
    modeSetMenit,
    modeSetTanggal,
    modeSetBulan,
    modeSetTahun,
    modeSetParameter,
  };

  void shiftOut(uint8_t value);
  void displaySholat(unsigned char sholat);

public:
  enum tampilParameter
  {
    KALENDER_ALARMBEEP,
    KALENDER_IQOMAH_SUBUH,
    KALENDER_IQOMAH_DZUHUR,
    KALENDER_IQOMAH_ASHAR,
    KALENDER_IQOMAH_MAGRIB,
    KALENDER_IQOMAH_ISYA,
    KALENDER_IQOMAH_JUMAT,
    KALENDER_STANBAY,
    KALENDER_ON,
    KALENDER_OFF,
    KALENDER_SETKOTA,
    KALENDER_RESET,
  };
  enum tampilKalender
  {
    kalenderSenin = 1,
    kalenderSelasa,
    kalenderRabu,
    kalenderKamis,
    kalenderJumat,
    kalenderSabtu,

    kalenderAhad,

    kalenderJadwal,
    kalenderTartil1,
    kalenderTartil2,
    kalenderTartil3,
    kalenderTartil4,
    kalenderTartil5,
    kalenderTartil6,
    kalenderKutbah,
  };
  Segmen();
  void loop();
  void setSqw(bool value);
  void setTime(unsigned char hour, unsigned char min);
  void setTanggal(unsigned char date, unsigned char mount, int year);
  void setImsya(unsigned char hour, unsigned char min);
  void setSubuh(unsigned char hour, unsigned char min);
  void setSuruq(unsigned char hour, unsigned char min);
  void setDzuhur(unsigned char hour, unsigned char min);
  void setAshar(unsigned char hour, unsigned char min);
  void setMaghrib(unsigned char hour, unsigned char min);
  void setIsya(unsigned char hour, unsigned char min);
  void displayHari(unsigned char day);
  void displayParameter(unsigned char parameter);
  void displayNormal();
  void displayTogleOff();
  void displayImsya();
  void displaySubuh();
  void displaySuruq();
  void displayDzuhur();
  void displayJumat();
  void displayAshar();
  void displayMaghrib();
  void displayIsya();
  void displayIqomah();

  void displaySetJam();
  void displaySetMenit();
  void displaySetTanggal();
  void displaySetBulan();
  void displaySetTahun();
  void displaySetParameter();
  void displayOff();
};
#endif
