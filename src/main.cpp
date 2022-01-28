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
#include "EpromStruct.h"
#include "Bluetooth.h"

void reset();
void baca_jadwal(int daerah);
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

#define text_iq_subuh 100
#define text_iq_duhur 200
#define text_iq_ashar 300
#define text_iq_maghrib 400
#define text_iq_isya 500
#define text_iq_jumat 600
#define text_run 700

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
EpromObject parameter; // Variable to store custom object read from EEPROM.
Jadwal jadwal = Jadwal();
Alamat alamat = Alamat();
Bluetooth myBluetooth = Bluetooth();
//==================================
// Var Global
volatile int alamat_eprom = 0;
unsigned char temp_min, temp_jam, temp_hri, rename_bt = 0; //, limaDetik;
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
      parameter.kota = 0;
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
  dmd.setBrightness(7);

  EEPROM.get(0, parameter);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  myDFPlayer.volume(26); // Set volume . From 0 to 30
  myDFPlayer.play(1);    // Play the first mp3
  // myDFPlayer.playFolder(2, 1);

  display_eprom(text_run);
  power.setTimeOn(parameter.timeOn);
  power.setTimeOff(parameter.timeOff);
  myTombol.setBuzer(&myBuzer);

  myBluetooth.setEprom(&parameter);
  myBluetooth.setBuzer(&myBuzer);
  myBluetooth.setRtc(&myRtc);
  myBluetooth.setPower(&power);
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
      baca_jadwal(parameter.kota);
      temp_jam = jam;
    }
    if (hari != temp_hri)
    {
      EEPROM.get(0, parameter);
      temp_hri = hari;
    }
    if (myBluetooth.available())
    {
      switch (myBluetooth.getStatus())
      {
      case 1:
        baca_jadwal(parameter.kota);
        break;
      case 2:
        alamat_eprom = 0;
        display_eprom(text_run);
        break;
      case 3:
        reset();
        baca_jadwal(parameter.kota);
        alamat_eprom = 0;
        display_eprom(text_run);
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
    dmd.setBrightness(7);
    tampil_text(text_run);
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

  // Serial.print("Start set kota =");
  // Serial.println(parameter.kota);
  myTombol.setMax(316);
  myTombol.setMin(0);
  myTombol.setValue(parameter.kota);
  mySegmen.displaySetParameter();
  mySegmen.displayParameter(mySegmen.KALENDER_SETKOTA);
  while (myTombol.getPos() == (200))
  {
    mySegmen.loop();
    tampil_text(text_run);
    myTombol.loop();
    mySegmen.setTime(parameter.kota / 100, parameter.kota % 100);
    if (event300ms.getEvent())
    {
      mySegmen.displayTogleOff();
    }
    parameter.kota = myTombol.getValue();
  }

  for (unsigned char i = 0; i < 5; i++)
  {
    myTombol.setMax(60);
    myTombol.setMin(-60);
    switch (i)
    {
    case 0:
      mySegmen.displaySubuh();
      jam = parameter.tambah_kurang_subuh;
      break;
    case 1:
      mySegmen.displayDzuhur();
      jam = parameter.tambah_kurang_duhur;
      break;
    case 2:
      mySegmen.displayAshar();
      jam = parameter.tambah_kurang_ashar;
      break;
    case 3:
      mySegmen.displayMaghrib();
      jam = parameter.tambah_kurang_maghrib;
      break;
    case 4:
      mySegmen.displayIsya();
      jam = parameter.tambah_kurang_isya;
      break;
    }
    myTombol.setValue(jam);
    while (myTombol.getPos() == (201 + i))
    {
      mySegmen.loop();
      tampil_text(text_run);
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
          parameter.tambah_kurang_subuh = jam;
          break;
        case 1:
          parameter.tambah_kurang_duhur = jam;
          break;
        case 2:
          parameter.tambah_kurang_ashar = jam;
          break;
        case 3:
          parameter.tambah_kurang_maghrib = jam;
          break;
        case 4:
          parameter.tambah_kurang_isya = jam;
          break;
        }
        baca_jadwal(parameter.kota);
      }
      jam = myTombol.getValue();
    }
  }

  mySegmen.displayOff();
  second.reset();
  while (myTombol.getPos() == 206)
  {
    tampil_text(text_run);
    myBuzer.loop();
    mySegmen.loop();
    if (second.getEvent())
    {
      break;
    }
  }
  EEPROM.put(0, parameter);
  baca_jadwal(parameter.kota);
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
  param[0] = parameter.beep;
  param[1] = parameter.iqomah_subuh;
  param[2] = parameter.iqomah_duhur;
  param[3] = parameter.iqomah_ashar;
  param[4] = parameter.iqomah_maghrib;
  param[5] = parameter.iqomah_isya;
  param[6] = parameter.iqomah_jumat;
  param[7] = parameter.lama_sholat_subuh;
  param[8] = parameter.timeOn;
  param[9] = parameter.timeOff;
  // param[10] = parameter.kota;

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
        tampil_text(text_run);
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
        tampil_text(text_run);
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
  parameter.beep = param[0];
  parameter.iqomah_subuh = param[1];
  parameter.iqomah_duhur = param[2];
  parameter.iqomah_ashar = param[3];
  parameter.iqomah_maghrib = param[4];
  parameter.iqomah_isya = param[5];
  parameter.iqomah_jumat = param[6];
  parameter.lama_sholat_subuh = param[7];
  parameter.lama_sholat_duhur = param[7];
  parameter.lama_sholat_ashar = param[7];
  parameter.lama_sholat_maghrib = param[7];
  parameter.lama_sholat_isya = param[7];
  parameter.lama_sholat_jumat = param[7];
  parameter.timeOn = param[8];
  parameter.timeOff = param[9];
  // parameter.kota = param[10];
  parameter.timer_adzan_subuh = 4;
  parameter.timer_adzan_duhur = 4;
  parameter.timer_adzan_ashar = 4;
  parameter.timer_adzan_maghrib = 4;
  parameter.timer_adzan_jumat = 4;
  parameter.timer_adzan_isya = 4;

  parameter.jadwal_fix_subuh = 0;
  parameter.jadwal_fix_duhur = 0;
  parameter.jadwal_fix_ashar = 0;
  parameter.jadwal_fix_maghrib = 0;
  parameter.jadwal_fix_isya = 0;

  parameter.tartil_subuh = 0;
  parameter.tartil_duhur = 0;
  parameter.tartil_ashar = 0;
  parameter.tartil_maghrib = 0;
  parameter.tartil_isya = 0;
  parameter.tartil_jumat = 0;

  EEPROM.put(0, parameter);
  second.reset();
  while (myTombol.getPos() == 110)
  {
    tampil_text(text_run);
    myBuzer.loop();
    mySegmen.loop();
    if (second.getEvent())
    {
      break;
    }
  }
  baca_jadwal(parameter.kota);
  mySegmen.displayNormal();
  myTombol.resetMenu();
  myBuzer.onRepeat(3, 100);
  // Serial.print("End set kota =");
  // Serial.println(parameter.kota);
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
    tampil_text(text_run);
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
    tampil_text(text_run);
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
    tampil_text(text_run);
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
    tampil_text(text_run);
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
    tampil_text(text_run);
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
    tampil_text(text_run);
    myBuzer.loop();
    mySegmen.loop();
    if (second.getEvent())
    {
      break;
    }
  }
  baca_jadwal(parameter.kota);
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
  // Serial.println("play = ");
  for (uint8_t i = 1; i < 6; i++)
  {
    bool play;
    play = jadwal.getAlarmByOffsite(i, myRtc.getJam(), myRtc.getMenit(), -10);

    // Serial.println(play);
    if (play)
    {
      myDFPlayer.playFolder(2, i);
      // myDFPlayer.playLargeFolder(2, i);
    }
  }
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
  uint8_t beep_alarm = parameter.beep;
  int time_adzan;
  int _alamat_text;
  int count_iqomah;
  int stanby_sholat;
  alamat_eprom = 0;
  // beep_alarm = parameter.beep; // parameter.beep;
  switch (jadwal.getAlarm())
  {
  case alamat.ALARM_IMSYA:
    time_adzan = 60;
    display_eprom(text_run);
    _alamat_text = text_run;
    mySegmen.displayImsya();
    break;
  case alamat.ALARM_SUBUH:
    time_adzan = parameter.timer_adzan_subuh * 60;
    count_iqomah = parameter.iqomah_subuh * 60;
    stanby_sholat = parameter.lama_sholat_subuh * 60;
    display_eprom(text_iq_subuh);
    _alamat_text = text_iq_subuh;
    mySegmen.displaySubuh();
    myDFPlayer.playFolder(1, 1); // play adzan
    break;
  case alamat.ALARM_SURUQ:
    time_adzan = 60;
    display_eprom(text_run);
    _alamat_text = text_run;
    mySegmen.displaySuruq();
    break;

  case alamat.ALARM_DZUHUR:
    time_adzan = parameter.timer_adzan_duhur * 60;
    count_iqomah = parameter.iqomah_duhur * 60;
    stanby_sholat = parameter.lama_sholat_duhur * 60;
    display_eprom(text_iq_duhur);
    _alamat_text = text_iq_duhur;
    mySegmen.displayDzuhur();
    myDFPlayer.playFolder(1, 2); // play adzan
    break;
  case alamat.ALARM_ASHAR:
    time_adzan = parameter.timer_adzan_ashar * 60;
    count_iqomah = parameter.iqomah_ashar * 60;
    stanby_sholat = parameter.lama_sholat_ashar * 60;
    display_eprom(text_iq_ashar);
    _alamat_text = text_iq_ashar;
    mySegmen.displayAshar();
    myDFPlayer.playFolder(1, 3); // play adzan
    break;
  case alamat.ALARM_MAGHRIB:
    time_adzan = parameter.timer_adzan_maghrib * 60;
    count_iqomah = parameter.iqomah_maghrib * 60;
    stanby_sholat = parameter.lama_sholat_maghrib * 60;
    display_eprom(text_iq_maghrib);
    _alamat_text = text_iq_maghrib;
    mySegmen.displayMaghrib();
    myDFPlayer.playFolder(1, 4); // play adzan
    break;
  case alamat.ALARM_ISYA:
    time_adzan = parameter.timer_adzan_isya * 60;
    count_iqomah = parameter.iqomah_isya * 60;
    stanby_sholat = parameter.lama_sholat_isya * 60;
    display_eprom(text_iq_isya);
    _alamat_text = text_iq_isya;
    mySegmen.displayIsya();
    myDFPlayer.playFolder(1, 5); // play adzan
    break;
  case alamat.ALARM_JUMAT:
    time_adzan = parameter.timer_adzan_jumat * 60;
    count_iqomah = parameter.iqomah_jumat * 60;
    stanby_sholat = parameter.lama_sholat_jumat * 60;
    display_eprom(text_iq_jumat);
    _alamat_text = text_iq_jumat;
    mySegmen.displayJumat();
    myDFPlayer.playFolder(1, 5); // play adzan
    break;
  default:
    break;
  }

  myBuzer.onRepeat(beep_alarm, 500);
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
  display_eprom(text_run);
  alamat_eprom = 0;
}
void baca_jadwal(int daerah)
{
  float lt = pgm_read_float(lintang + daerah);
  float bj = pgm_read_float(bujur + daerah);
  unsigned char jam, menit;
  unsigned char wkt = pgm_read_byte_near(gmt + daerah);
  if (daerah == 0)
  {
    wkt = parameter.set_kota_gmt;
    lt = parameter.set_kota_lnt;
    bj = parameter.set_kota_bjr;
  }
  // baca jadwal
  jadwal.setOffsiteSubuh(parameter.tambah_kurang_subuh);
  jadwal.setOffsiteDzuhur(parameter.tambah_kurang_duhur);
  jadwal.setOffsiteAshar(parameter.tambah_kurang_ashar);
  jadwal.setOffsiteMaghrib(parameter.tambah_kurang_maghrib);
  jadwal.setOffsiteIsya(parameter.tambah_kurang_isya);
  // set fix jadwal
  jadwal.setFixSubuh(parameter.jadwal_fix_subuh);
  jadwal.setFixDzuhur(parameter.jadwal_fix_duhur);
  jadwal.setFixAshar(parameter.jadwal_fix_ashar);
  jadwal.setFixMaghrib(parameter.jadwal_fix_maghrib);
  jadwal.setFixIsya(parameter.jadwal_fix_isya);
  jadwal.setZona(wkt);
  jadwal.setBujur(bj);
  jadwal.setLintang(lt);
  jadwal.setTanggal(myRtc.getTanggal(), myRtc.getBulan(), myRtc.getTahun());
  jadwal.getImsya(jam, menit);
  mySegmen.setImsya(jam, menit);
  jadwal.getSubuh(jam, menit);
  mySegmen.setSubuh(jam, menit);
  jadwal.getSuruq(jam, menit);
  mySegmen.setSuruq(jam, menit);
  jadwal.getDzuhur(jam, menit);
  mySegmen.setDzuhur(jam, menit);
  jadwal.getAshar(jam, menit);
  mySegmen.setAshar(jam, menit);
  jadwal.getMaghrib(jam, menit);
  mySegmen.setMaghrib(jam, menit);
  jadwal.getIsya(jam, menit);
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
  // parameter.kota = 0;
  parameter.beep = 10;
  parameter.timer_adzan_subuh = 4;
  parameter.timer_adzan_duhur = 4;
  parameter.timer_adzan_ashar = 4;
  parameter.timer_adzan_maghrib = 4;
  parameter.timer_adzan_isya = 4;
  parameter.timer_adzan_jumat = 4;
  parameter.iqomah_subuh = 12;
  parameter.iqomah_duhur = 12;
  parameter.iqomah_ashar = 12;
  parameter.iqomah_maghrib = 12;
  parameter.iqomah_isya = 12;
  parameter.iqomah_jumat = 25;
  parameter.lama_sholat_subuh = 5;
  parameter.lama_sholat_duhur = 5;
  parameter.lama_sholat_ashar = 5;
  parameter.lama_sholat_maghrib = 5;
  parameter.lama_sholat_isya = 5;
  parameter.lama_sholat_jumat = 5;
  parameter.tartil_subuh = 0; // 0= mati dan >0 hidup
  parameter.tartil_duhur = 0;
  parameter.tartil_ashar = 0;
  parameter.tartil_maghrib = 0;
  parameter.tartil_isya = 0;
  parameter.tartil_jumat = 0;
  parameter.kecerahan_1 = 1;
  parameter.kecerahan_2 = 1;
  parameter.kecerahan_3 = 1;
  parameter.kecerahan_4 = 1;
  parameter.jam_kecerahan_1 = 1;
  parameter.jam_kecerahan_2 = 1;
  parameter.jam_kecerahan_3 = 1;
  parameter.jam_kecerahan_4 = 1;
  parameter.tambah_kurang_subuh = 0;
  parameter.tambah_kurang_duhur = 0;
  parameter.tambah_kurang_ashar = 0;
  parameter.tambah_kurang_maghrib = 0;
  parameter.tambah_kurang_isya = 0;
  parameter.jadwal_fix_subuh = 0;
  parameter.jadwal_fix_duhur = 0;
  parameter.jadwal_fix_ashar = 0;
  parameter.jadwal_fix_maghrib = 0;
  parameter.jadwal_fix_isya = 0;
  parameter.mazhab_ashar = 0;
  parameter.set_kota_gmt = 7;
  parameter.set_kota_lnt = -7.80;
  parameter.set_kota_bjr = 110.400;
  parameter.timeOn = 3 * 60;
  parameter.timeOff = 22 * 60;
  EEPROM.put(0, parameter);
  myBuzer.setOff();
  for (int t = 0; t < 420; t++)
  {
    char chr = pgm_read_byte_near(runningtext + t);
    EEPROM.write(text_run + t, chr);
  }
  for (int n = 0; n < 20; n++)
  { // for(i=0;i<20;i++){
    char chr = pgm_read_byte_near(textsholat_sb + n);
    EEPROM.write(text_iq_subuh + n, chr);
  }

  for (int n = 0; n < 21; n++)
  { // for(i=0;i<21;i++){
    char chr = pgm_read_byte_near(textsholat_dh + n);
    EEPROM.write(text_iq_duhur + n, chr);
  }

  for (int n = 0; n < 20; n++)
  { // for(i=0;i<20;i++){
    char chr = pgm_read_byte_near(textsholat_as + n);
    EEPROM.write(text_iq_ashar + n, chr);
  }

  for (int n = 0; n < 22; n++)
  { // for(i=0;i<22;i++){
    char chr = pgm_read_byte_near(textsholat_mg + n);
    EEPROM.write(text_iq_maghrib + n, chr);
  }

  for (int n = 0; n < 19; n++)
  { // for(int i=0;i<19;i++){
    char chr = pgm_read_byte_near(textsholat_is + n);
    EEPROM.write(text_iq_isya + n, chr);
  }

  for (int n = 0; n < 70; n++)
  { // for(i=0;i<70;i++){
    char chr = pgm_read_byte_near(textsholat_jm + n);
    EEPROM.write(text_iq_jumat + n, chr);
  }
}
