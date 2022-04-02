#define SERIAL_RX_BUFFER_SIZE 512
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include "kota.h"
#include "TimerThree.h"
#include "DFRobotDFPlayerMini.h"
#include "DMD.h"
#include "myfont.h"
#include "constan.h"
#include "power.h"
#include "Buzer.h"
#include "Rtc.h"
#include "Segmen.h"
#include "Event.h"
#include "Jadwal.h"
#include "Alamat.h"

#include "Tombol.h"
#include "Memory.h"
#include "Bluetooth.h"
#include "Tilawah.h"

void reset();
void baca_jadwal();
void display_eprom(int add);
void tampil_text(int _alamat_text);
void alarm_on();
void alarm();
void setJam();
void setOffsite();
void setParameter();
//====================================
#define bluetooth 9600 // 38400              //jika hc 06 atau hc05 ganti dengan 38400
#define runingTextSpeed 40
#define namaBluetooth "Jws Basmalla"
#define DISPLAYS_ACROSS 2
#define DISPLAYS_DOWN 1


#define pinLampu 6
#define LAMPUON HIGH
#define LAMPUOFF LOW
#define buzerPin 2
#define tombol_up 5
#define tombol_menu 45
#define tombol_down 46

// Object Declaration
Tombol myTombol = Tombol(tombol_menu, tombol_up, tombol_down);
Power power = Power();
Event second = Event(1000);
Event event300ms = Event(300);
Event textSpeed = Event(runingTextSpeed);
Buzer myBuzer = Buzer(buzerPin);
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
Rtc myRtc = Rtc();
Segmen mySegmen = Segmen();
DFRobotDFPlayerMini myDFPlayer;
Memory eeprom = Memory();
Jadwal jadwal = Jadwal();
Alamat alamat = Alamat();
Bluetooth myBluetooth = Bluetooth();
Tilawah tilawah = Tilawah();
//==================================
// Var Global
volatile int alamat_eprom = 0;
unsigned char temp_min, temp_jam; //, limaDetik;
unsigned char jam, menit, hari;

void ScanDMD()
{
  dmd.scanDisplayBySPI();
}
void setup()
{
  pinMode(tombol_up, INPUT_PULLUP);
  pinMode(tombol_menu, INPUT_PULLUP);
  pinMode(tombol_down, INPUT_PULLUP);
  pinMode(pinLampu, OUTPUT);
  dmd.clearScreen(true);
  Timer3.initialize(500); // period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
  Timer3.attachInterrupt(ScanDMD);
  for (unsigned char i = 0; i < 3; i++)
  {
    myBuzer.setOn();
    delay(250);
    myBuzer.setOff();
    delay(250);
    myTombol.loop();
    if (myTombol.getPos() == 200)
    {
      eeprom.setKota(0);
      reset();
      myTombol.resetMenu();
      break;
    }
  }
  Serial1.begin(bluetooth);
  Serial.begin(9600);
  Wire.begin();
  Serial1.setTimeout(200);
  myDFPlayer.begin(Serial);
  myDFPlayer.setTimeOut(500);
  dmd.selectFont(myFont);
  myDFPlayer.reset();
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  myDFPlayer.EQ(eeprom.getEqualizer());     // Set equalizer From 0 to 5
  myDFPlayer.volume(eeprom.getVolumemp3()); // Set volume From 0 to 30
  myDFPlayer.play(1);                       // Play the first mp3
  display_eprom(alamat.TEXT_RUN);
  power.setTimeOn(eeprom.getTimeOn());
  power.setTimeOff(eeprom.getTimeOff());
  myTombol.setBuzer(&myBuzer);

  myBluetooth.setEprom(&eeprom);
  myBluetooth.setBuzer(&myBuzer);
  myBluetooth.setRtc(&myRtc);
  myBluetooth.setPower(&power);
  myBluetooth.setPlayer(&myDFPlayer);

  tilawah.setPlayer(&myDFPlayer);
  tilawah.setEprom(&eeprom);
  myRtc.readAll();
  baca_jadwal();
  Serial1.print("AT+NAME");
  Serial1.println(namaBluetooth);
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void loop()
{
  if (second.getEvent())
  {
    myRtc.readAll();
    mySegmen.setTime(myRtc.getJam(), myRtc.getMenit());
    mySegmen.setTanggal(myRtc.getTanggal(), myRtc.getBulan(), myRtc.getTahun());
    power.setTime(myRtc.getJam(), myRtc.getMenit());
    menit = myRtc.getMenit();
    jam = myRtc.getJam();
    hari = myRtc.getHari();
    if (menit != temp_min)
    {
      alarm();
      temp_min = menit;
    }
    if (jam != temp_jam)
    {
      baca_jadwal();
      temp_jam = jam;
    }
    if (myBluetooth.available())
    {
      switch (myBluetooth.getStatus())
      {
      case 1:
        baca_jadwal();
        break;
      case 2:
        alamat_eprom = 0;
        display_eprom(alamat.TEXT_RUN);
        break;
      case 3:
        reset();
        baca_jadwal();
        alamat_eprom = 0;
        display_eprom(alamat.TEXT_RUN);
        break;
      }
    }
  }
  if ((myRtc.getDetik() % 10) < 5)
  {
    mySegmen.displayHari(myRtc.getHari());
  }

  if (power.getState() == true) // hidup
  {
    mySegmen.displayNormal();
    tampil_text(alamat.TEXT_RUN);
  }
  else // mati
  {
    dmd.clearScreen(true);
    mySegmen.displayOff();
  }
  myTombol.loop();
  mySegmen.loop();
  switch (myTombol.getPos())
  {
  case 1:
    setJam();
    break;
  case 100:
    setParameter();
    break;
  case 200:
    setOffsite();
    break;
  default:
    myTombol.resetMenu();
    break;
  }
}

/***********************************************************************************/

void setOffsite()
{

  int jam = 0;
  bool brek = myTombol.getDownLongPress(5000);
  if (brek == false)
  {
    return;
  }
  myBuzer.onOnce(200);
  myTombol.setMax(316);
  myTombol.setMin(0);
  jam = eeprom.getKota();
  myTombol.setValue(jam);
  mySegmen.displaySetParameter();
  mySegmen.displayParameter(mySegmen.KALENDER_SETKOTA);
  while (myTombol.getPos() == (200))
  {
    mySegmen.loop();
    tampil_text(alamat.TEXT_RUN);
    myTombol.loop();
    mySegmen.setTime(jam / 100, jam % 100);
    if (event300ms.getEvent())
    {
      mySegmen.displayTogleOff();
    }
    jam = myTombol.getValue();
  }
  eeprom.setKota(jam);

  for (unsigned char i = 0; i < 5; i++)
  {
    myTombol.setMax(60);
    myTombol.setMin(-60);
    switch (i)
    {
    case 0:
      mySegmen.displaySubuh();
      jam = eeprom.getTambahKurangSubuh();
      break;
    case 1:
      mySegmen.displayDzuhur();
      jam = eeprom.getTambahKurangDuhur();
      break;
    case 2:
      mySegmen.displayAshar();
      jam = eeprom.getTambahKurangAshar();
      break;
    case 3:
      mySegmen.displayMaghrib();
      jam = eeprom.getTambahKurangMaghrib();
      break;
    case 4:
      mySegmen.displayIsya();
      jam = eeprom.getTambahKurangIsya();
      break;
    }
    myTombol.setValue(jam);
    while (myTombol.getPos() == (201 + i))
    {
      mySegmen.loop();
      tampil_text(alamat.TEXT_RUN);
      myTombol.loop();
      mySegmen.setTime(0, abs(jam));
      if (jam < 0)
      {
        mySegmen.setNegative();
      }

      if (event300ms.getEvent())
      {
        mySegmen.displayTogleOff();
        switch (i)
        {
        case 0:
          eeprom.setTambahKurangSubuh(jam);
          break;
        case 1:
          eeprom.setTambahKurangDuhur(jam);
          break;
        case 2:
          eeprom.setTambahKurangAshar(jam);
          break;
        case 3:
          eeprom.setTambahKurangMaghrib(jam);
          break;
        case 4:
          eeprom.setTambahKurangIsya(jam);
          break;
        }
        baca_jadwal();
      }
      jam = myTombol.getValue();
    }
  }

  mySegmen.displayOff();
  second.reset();
  while (myTombol.getPos() == 206)
  {
    tampil_text(alamat.TEXT_RUN);
    myBuzer.loop();
    mySegmen.loop();
    if (second.getEvent())
    {
      break;
    }
  }
  baca_jadwal();
  mySegmen.displayNormal();
  myTombol.resetMenu();
  myBuzer.onRepeat(3, 100);
}

/***********************************************************************************/

void setParameter()
{
  unsigned char jam = 00;
  // set parameter
  int param[10];
  param[0] = eeprom.getBeep();
  param[1] = eeprom.getIqomahSubuh();
  param[2] = eeprom.getIqomahDuhur();
  param[3] = eeprom.getIqomahAshar();
  param[4] = eeprom.getIqomahMaghrib();
  param[5] = eeprom.getIqomahIsya();
  param[6] = eeprom.getIqomahJumat();
  param[7] = eeprom.getLamaSholatSubuh();
  param[8] = eeprom.getTimeOn();
  param[9] = eeprom.getTimeOff();
  for (unsigned char i = 0; i < 10; i++)
  {
    if (i < 8)
    {
      myTombol.setMax(99);
      myTombol.setMin(0);
      myTombol.setValue(param[i]);
      mySegmen.displaySetMenit();
      mySegmen.displayParameter(mySegmen.KALENDER_ALARMBEEP + i);
      while (myTombol.getPos() == (100 + i))
      {
        mySegmen.loop();
        tampil_text(alamat.TEXT_RUN);
        myTombol.loop();
        mySegmen.setTime(jam, param[i]);
        if (event300ms.getEvent())
        {
          mySegmen.displayTogleOff();
        }
        param[i] = myTombol.getValue();
      }
    }
    else
    {
      jam = param[i] / 60;
      myTombol.setMax(23);
      myTombol.setMin(0);
      myTombol.setValue(jam);
      mySegmen.displaySetParameter();
      mySegmen.displayParameter(mySegmen.KALENDER_ALARMBEEP + i);
      while (myTombol.getPos() == (100 + i))
      {
        mySegmen.loop();
        tampil_text(alamat.TEXT_RUN);
        myTombol.loop();
        mySegmen.setTime(jam, 0);
        if (event300ms.getEvent())
        {
          mySegmen.displayTogleOff();
        }
        jam = myTombol.getValue();
      }
      param[i] = jam * 60;
    }
  }
  mySegmen.displayParameter(mySegmen.KALENDER_RESET);
  // end set param
  mySegmen.displayOff();

  eeprom.setBeep(param[0]);
  eeprom.setIqomahSubuh(param[1]);
  eeprom.setIqomahDuhur(param[2]);
  eeprom.setIqomahAshar(param[3]);
  eeprom.setIqomahMaghrib(param[4]);
  eeprom.setIqomahIsya(param[5]);
  eeprom.setIqomahJumat(param[6]);
  eeprom.setLamaSholatSubuh(param[7]);
  eeprom.setLamaSholatDuhur(param[7]);
  eeprom.setLamaSholatAshar(param[7]);
  eeprom.setLamaSholatMaghrib(param[7]);
  eeprom.setLamaSholatIsya(param[7]);
  eeprom.setLamaSholatJumat(param[7]);
  eeprom.setTimeOn(param[8]);
  eeprom.setTimeOff(param[9]);
  eeprom.setTimerAdzanSubuh(4);
  eeprom.setTimerAdzanDuhur(4);
  eeprom.setTimerAdzanAshar(4);
  eeprom.setTimerAdzanMaghrib(4);
  eeprom.setTimerAdzanIsya(4);
  eeprom.setTimerAdzanJumat(4);
  eeprom.setJadwalFixSubuh(0);
  eeprom.setJadwalFixDzuhur(0);
  eeprom.setJadwalFixAshar(0);
  eeprom.setJadwalFixMaghrib(0);
  eeprom.setJadwalFixIsya(0);
  second.reset();
  while (myTombol.getPos() == 110)
  {
    tampil_text(alamat.TEXT_RUN);
    myBuzer.loop();
    mySegmen.loop();
    if (second.getEvent())
    {
      break;
    }
  }
  baca_jadwal(); // PARAMETER.kota);
  mySegmen.displayNormal();
  myTombol.resetMenu();
  myBuzer.onRepeat(3, 100);
}
void setJam()
{
  unsigned char jam = myRtc.getJam();
  unsigned char menit = myRtc.getMenit();
  unsigned char tanggal = myRtc.getTanggal();
  unsigned char bulan = myRtc.getBulan();
  int tahun = myRtc.getTahun();
  mySegmen.setTanggal(tanggal, bulan, tahun);
  // set jam
  myTombol.setMax(23);
  myTombol.setMin(0);
  myTombol.setValue(jam);
  mySegmen.displaySetJam();
  while (myTombol.getPos() == 1)
  {
    mySegmen.loop();
    tampil_text(alamat.TEXT_RUN);
    myTombol.loop();
    mySegmen.setTime(jam, menit);
    if (event300ms.getEvent())
    {
      mySegmen.displayTogleOff();
    }
    jam = myTombol.getValue();
  }
  // set menit
  myTombol.setMax(59);
  myTombol.setMin(0);
  myTombol.setValue(menit);
  mySegmen.displayNormal();
  mySegmen.displaySetMenit();
  while (myTombol.getPos() == 2)
  {
    mySegmen.loop();
    tampil_text(alamat.TEXT_RUN);
    myTombol.loop();
    mySegmen.setTime(jam, menit);

    if (event300ms.getEvent())
    {
      mySegmen.displayTogleOff();
    }
    menit = myTombol.getValue();
  }
  myRtc.setJam(jam, menit, 0);
  // set tanggal
  myTombol.setMax(31);
  myTombol.setMin(1);
  myTombol.setValue(tanggal);
  mySegmen.displayNormal();
  mySegmen.displaySetTanggal();
  while (myTombol.getPos() == 3)
  {
    mySegmen.loop();
    tampil_text(alamat.TEXT_RUN);
    myTombol.loop();
    mySegmen.setTanggal(tanggal, bulan, tahun);
    if (event300ms.getEvent())
    {
      mySegmen.displayTogleOff();
    }
    tanggal = myTombol.getValue();
  }
  // set bulan
  myTombol.setMax(12);
  myTombol.setMin(1);
  myTombol.setValue(bulan);
  mySegmen.displayNormal();
  mySegmen.displaySetBulan();
  while (myTombol.getPos() == 4)
  {
    mySegmen.loop();
    tampil_text(alamat.TEXT_RUN);
    myTombol.loop();
    mySegmen.setTanggal(tanggal, bulan, tahun);
    if (event300ms.getEvent())
    {
      mySegmen.displayTogleOff();
    }
    bulan = myTombol.getValue();
  }
  // set Tahun
  myTombol.setMax(2099);
  myTombol.setMin(2020);
  myTombol.setValue(tahun);
  mySegmen.displayNormal();
  mySegmen.displaySetTahun();
  while (myTombol.getPos() == 5)
  {
    mySegmen.loop();
    tampil_text(alamat.TEXT_RUN);
    myTombol.loop();
    mySegmen.setTanggal(tanggal, bulan, tahun);
    if (event300ms.getEvent())
    {
      mySegmen.displayTogleOff();
    }
    tahun = myTombol.getValue();
  }

  myRtc.setTanggal(tanggal, bulan, tahun);

  mySegmen.displayOff();
  second.reset();
  while (myTombol.getPos() == 6)
  {
    tampil_text(alamat.TEXT_RUN);
    myBuzer.loop();
    mySegmen.loop();
    if (second.getEvent())
    {
      break;
    }
  }
  baca_jadwal();
  mySegmen.displayNormal();
  myTombol.resetMenu();
  myBuzer.onRepeat(3, 100);
}
void alarm(void)
{
  // lampu background on sebelum 15 menit
  for (uint8_t i = 1; i < 6; i++)
  {
    bool lampu = jadwal.getAlarmByOffsite(i, myRtc.getJam(), myRtc.getMenit(), -15);
    if (lampu)
    {
      digitalWrite(pinLampu, LAMPUON); // hidupkan lapu
    }
  }
  // lampu background  off setelah 30 menit
  for (uint8_t i = 1; i < 6; i++)
  {
    bool lampu = jadwal.getAlarmByOffsite(i, myRtc.getJam(), myRtc.getMenit(), 30);
    if (lampu)
    {
      digitalWrite(pinLampu, LAMPUOFF); // matikan lapu
    }
  }
  // play mp3 tilawah
  tilawah.setHari(myRtc.getHari());
  tilawah.setTime(myRtc.getJam(), myRtc.getMenit());
  // Masuk waktu sholat
  jadwal.setJam(myRtc.getJam(), myRtc.getMenit());
  jadwal.setHari(myRtc.getHari());
  unsigned char alaram = jadwal.getAlarm();
  if (alaram != alamat.ALARM_OFF)
  {
    alarm_on();
  }
}

void alarm_on()
{ // time out adzan
  myDFPlayer.stop();
  int time_adzan;
  int _alamat_text;
  int count_iqomah;
  int stanby_sholat;
  alamat_eprom = 0;
  switch (jadwal.getAlarm())
  {
  case alamat.ALARM_IMSYA:
    time_adzan = 60;
    display_eprom(alamat.TEXT_RUN);
    _alamat_text = alamat.TEXT_RUN;
    mySegmen.displayImsya();
    break;
  case alamat.ALARM_SUBUH:
    tilawah.playAdzanSubuh();
    time_adzan = eeprom.getTimerAdzanSubuh() * 60;
    count_iqomah = eeprom.getIqomahSubuh() * 60;
    stanby_sholat = eeprom.getLamaSholatSubuh() * 60;
    display_eprom(alamat.TEXT_IQ_SUBUH);
    _alamat_text = alamat.TEXT_IQ_SUBUH;
    mySegmen.displaySubuh();
    break;
  case alamat.ALARM_SURUQ:
    time_adzan = 60;
    display_eprom(alamat.TEXT_RUN);
    _alamat_text = alamat.TEXT_RUN;
    mySegmen.displaySuruq();
    break;

  case alamat.ALARM_DZUHUR:
    tilawah.playAdzanDzuhur();
    time_adzan = eeprom.getTimerAdzanDuhur() * 60;
    count_iqomah = eeprom.getIqomahDuhur() * 60;
    stanby_sholat = eeprom.getLamaSholatDuhur() * 60;
    display_eprom(alamat.TEXT_IQ_DUHUR);
    _alamat_text = alamat.TEXT_IQ_DUHUR;
    mySegmen.displayDzuhur();
    break;
  case alamat.ALARM_ASHAR:
    tilawah.playAdzanAshar();
    time_adzan = eeprom.getTimerAdzanAshar() * 60;
    count_iqomah = eeprom.getIqomahAshar() * 60;
    stanby_sholat = eeprom.getLamaSholatAshar() * 60;
    display_eprom(alamat.TEXT_IQ_ASHAR);
    _alamat_text = alamat.TEXT_IQ_ASHAR;
    mySegmen.displayAshar();
    break;
  case alamat.ALARM_MAGHRIB:
    tilawah.playAdzanMaghrib();
    time_adzan = eeprom.getTimerAdzanMaghrib() * 60;
    count_iqomah = eeprom.getIqomahMaghrib() * 60;
    stanby_sholat = eeprom.getLamaSholatMaghrib() * 60;
    display_eprom(alamat.TEXT_IQ_MAGHRIB);
    _alamat_text = alamat.TEXT_IQ_MAGHRIB;
    mySegmen.displayMaghrib();
    break;
  case alamat.ALARM_ISYA:
    tilawah.playAdzanIsya();
    time_adzan = eeprom.getTimerAdzanIsya() * 60;
    count_iqomah = eeprom.getIqomahIsya() * 60;
    stanby_sholat = eeprom.getLamaSholatIsya() * 60;
    display_eprom(alamat.TEXT_IQ_ISYA);
    _alamat_text = alamat.TEXT_IQ_ISYA;
    mySegmen.displayIsya();
    break;
  case alamat.ALARM_JUMAT:
    tilawah.playAdzanJumat();
    time_adzan = eeprom.getTimerAdzanJumat() * 60;
    count_iqomah = eeprom.getIqomahJumat() * 60;
    stanby_sholat = eeprom.getLamaSholatJumat() * 60;
    display_eprom(alamat.TEXT_IQ_JUMAT);
    _alamat_text = alamat.TEXT_IQ_JUMAT;
    mySegmen.displayJumat();
    break;
  }

  myBuzer.onRepeat(eeprom.getBeep(), 500);
  while (time_adzan)
  {
    if (second.getEvent())
    {
      time_adzan--;
      myRtc.readAll();
      mySegmen.setTime(myRtc.getJam(), myRtc.getMenit());
    }
    if (event300ms.getEvent())
    {
      mySegmen.displayTogleOff();
    }
    myBuzer.loop();
    mySegmen.loop();
    myBluetooth.loop();
    tampil_text(_alamat_text);
  }
  // time iqomah count down
  if (jadwal.getAlarm() == alamat.ALARM_SURUQ)
  {
    return;
  }
  if (jadwal.getAlarm() == alamat.ALARM_IMSYA)
  {
    return;
  }

  mySegmen.displayIqomah(); // tampil_hari(iqomah);
  myBuzer.onOnce(250);

  if (count_iqomah > 0)
  {
    while (count_iqomah >= 0)
    {
      myBuzer.loop();
      mySegmen.loop();
      myBluetooth.loop();
      tampil_text(_alamat_text);
      if ((millis() % 1000) < 500)
      {
        if (second.getEvent())
        {
          count_iqomah--;
          mySegmen.setTime(count_iqomah / 60, count_iqomah % 60);
          if (count_iqomah == 1)
          {
            myBuzer.onRepeat(2, 500);
          }
        }
        mySegmen.displayIqomah();
      }
      else
      {
        mySegmen.displayOff();
      }
    }
    myBuzer.setOn();
    delay(2000);
    myBuzer.setOff();
  }
  mySegmen.displayOff();
  dmd.clearScreen(true);
  while (stanby_sholat > 0)
  {
    if (second.getEvent())
    {
      myRtc.readAll();
      mySegmen.setTime(myRtc.getJam(), myRtc.getMenit());
      stanby_sholat--;
    }
    mySegmen.loop();
    myBluetooth.loop();
    if ((millis() % 1000) < 500)
    {
      dmd.drawChar(59, 0, ':', GRAPHICS_NORMAL);
      dmd.drawChar(66, 0, ':', GRAPHICS_NORMAL);
    }
    else
    {
      dmd.clearScreen(true);
    }
  }
  display_eprom(alamat.TEXT_RUN);
  alamat_eprom = 0;
}
void baca_jadwal()
{
  int daerah = eeprom.getKota();
  float lt = pgm_read_float(lintang + daerah);
  float bj = pgm_read_float(bujur + daerah);
  unsigned char jam, menit;
  unsigned char wkt = pgm_read_byte_near(gmt + daerah);
  if (daerah == 0)
  {
    wkt = eeprom.getKotaGMT();
    lt = eeprom.getKotaLintang();
    bj = eeprom.getKotaBujur();
  }
  // baca jadwal
  jadwal.setOffsiteSubuh(eeprom.getTambahKurangSubuh());
  jadwal.setOffsiteDzuhur(eeprom.getTambahKurangDuhur());
  jadwal.setOffsiteAshar(eeprom.getTambahKurangAshar());
  jadwal.setOffsiteMaghrib(eeprom.getTambahKurangMaghrib());
  jadwal.setOffsiteIsya(eeprom.getTambahKurangIsya());
  // set fix jadwal
  jadwal.setFixSubuh(eeprom.getJadwalFixSubuh());
  jadwal.setFixDzuhur(eeprom.getJadwalFixDzuhur());
  jadwal.setFixAshar(eeprom.getJadwalFixAshar());
  jadwal.setFixMaghrib(eeprom.getJadwalFixMaghrib());
  jadwal.setFixIsya(eeprom.getJadwalFixIsya());
  jadwal.setZona(wkt);
  jadwal.setBujur(bj);
  jadwal.setLintang(lt);
  jadwal.setTanggal(myRtc.getTanggal(), myRtc.getBulan(), myRtc.getTahun());
  jadwal.getImsya(jam, menit);
  mySegmen.setImsya(jam, menit);
  jadwal.getSubuh(jam, menit);
  tilawah.setTimeSubuh(jam, menit);
  mySegmen.setSubuh(jam, menit);
  jadwal.getSuruq(jam, menit);
  mySegmen.setSuruq(jam, menit);
  jadwal.getDzuhur(jam, menit);
  tilawah.setTimeDzuhur(jam, menit);
  mySegmen.setDzuhur(jam, menit);
  jadwal.getAshar(jam, menit);
  tilawah.setTimeAshar(jam, menit);
  mySegmen.setAshar(jam, menit);
  jadwal.getMaghrib(jam, menit);
  tilawah.setTimeMaghrib(jam, menit);
  mySegmen.setMaghrib(jam, menit);
  jadwal.getIsya(jam, menit);
  tilawah.setTimeIsya(jam, menit);
  mySegmen.setIsya(jam, menit);
}
void tampil_text(int _alamat_text)
{
  if (textSpeed.getEvent())
  {
    if (dmd.stepMarquee(-1, 0))
    {
      display_eprom(_alamat_text);
    }
  }
}
//

//==================================================================================
void serialEvent1()
{
  myBluetooth.loop();
}

void display_eprom(int add)
{
  int len = 0;
  char run_data[512]; // Max 512 Bytes
  unsigned char k;
  k = EEPROM.read(add);
  if (alamat_eprom >= 500)
  {
    alamat_eprom = 0;
  }
  while (alamat_eprom < 500)
  { // Read until null character
    k = EEPROM.read(add + alamat_eprom);
    run_data[len] = k;
    alamat_eprom++;
    len++;
    if (k == '\n')
    {
      run_data[len] = '\0';
      dmd.clearScreen(true);
      dmd.drawMarquee(run_data, len, (64 * DISPLAYS_ACROSS) - 1, 0);
      k = EEPROM.read(add + alamat_eprom);
      if (k == '\0')
        alamat_eprom = 0;
      break;
    }
    if (k == '\0')
    {
      alamat_eprom = 0;
      break;
    }
  }
}

void reset()
{
  eeprom.setKota(0);
  eeprom.setBeep(10);
  eeprom.setTimerAdzanSubuh(4);
  eeprom.setTimerAdzanDuhur(4);
  eeprom.setTimerAdzanAshar(4);
  eeprom.setTimerAdzanMaghrib(4);
  eeprom.setTimerAdzanIsya(4);
  eeprom.setTimerAdzanJumat(4);
  eeprom.setIqomahSubuh(12);
  eeprom.setIqomahDuhur(12);
  eeprom.setIqomahAshar(12);
  eeprom.setIqomahMaghrib(12);
  eeprom.setIqomahIsya(12);
  eeprom.setIqomahJumat(25);
  eeprom.setLamaSholatSubuh(5);
  eeprom.setLamaSholatDuhur(5);
  eeprom.setLamaSholatAshar(5);
  eeprom.setLamaSholatMaghrib(5);
  eeprom.setLamaSholatIsya(5);
  eeprom.setLamaSholatJumat(5);
  eeprom.setTambahKurangSubuh(0);
  eeprom.setTambahKurangDuhur(0);
  eeprom.setTambahKurangAshar(0);
  eeprom.setTambahKurangMaghrib(0);
  eeprom.setTambahKurangIsya(0);
  eeprom.setJadwalFixSubuh(0);
  eeprom.setJadwalFixDzuhur(0);
  eeprom.setJadwalFixAshar(0);
  eeprom.setJadwalFixMaghrib(0);
  eeprom.setJadwalFixIsya(0);
  eeprom.setKotaGMT(7);
  eeprom.setKotaLintang(-7.80);
  eeprom.setKotaBujur(110.400);
  eeprom.setTimeOn(3 * 60);
  eeprom.setTimeOff(22 * 60);
  eeprom.setTimeTilawahSubuh(10);
  eeprom.setTimeTilawahDzuhur(10);
  eeprom.setTimeTilawahAshar(10);
  eeprom.setTimeTilawahMaghrib(10);
  eeprom.setTimeTilawahIsya(10);
  eeprom.setTimeTilawahJumat(20);

  myBuzer.setOff();
  for (int t = 0; t < 420; t++)
  {
    char chr = pgm_read_byte_near(runningtext + t);
    EEPROM.write(alamat.TEXT_RUN + t, chr);
  }
  delay(10);
  for (int n = 0; n < 20; n++)
  { // for(i=0;i<20;i++){
    char chr = pgm_read_byte_near(textsholat_sb + n);
    EEPROM.write(alamat.TEXT_IQ_SUBUH + n, chr);
  }
  delay(10);

  for (int n = 0; n < 21; n++)
  { // for(i=0;i<21;i++){
    char chr = pgm_read_byte_near(textsholat_dh + n);
    EEPROM.write(alamat.TEXT_IQ_DUHUR + n, chr);
  }
  delay(10);

  for (int n = 0; n < 20; n++)
  { // for(i=0;i<20;i++){
    char chr = pgm_read_byte_near(textsholat_as + n);
    EEPROM.write(alamat.TEXT_IQ_ASHAR + n, chr);
  }

  delay(10);
  for (int n = 0; n < 22; n++)
  { // for(i=0;i<22;i++){
    char chr = pgm_read_byte_near(textsholat_mg + n);
    EEPROM.write(alamat.TEXT_IQ_MAGHRIB + n, chr);
  }

  delay(10);
  for (int n = 0; n < 19; n++)
  { // for(int i=0;i<19;i++){
    char chr = pgm_read_byte_near(textsholat_is + n);
    EEPROM.write(alamat.TEXT_IQ_ISYA + n, chr);
  }

  delay(10);
  for (int n = 0; n < 70; n++)
  { // for(i=0;i<70;i++){
    char chr = pgm_read_byte_near(textsholat_jm + n);
    EEPROM.write(alamat.TEXT_IQ_JUMAT + n, chr);
  }
}
