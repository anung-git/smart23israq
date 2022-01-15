#define SERIAL_RX_BUFFER_SIZE 512
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include "var.h"
#include "PrayerTimes.h"
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

void reset();
void baca_jadwal(int daerah);
void display_eprom(int add);
void tampil_text(int _alamat_text);
void alarm_on(unsigned char sholat);
void alarm();
// void tombol();

//====================================
#define bluetooth 9600 // 38400              //jika hc 06 atau hc05 ganti dengan 38400
#define runingTextSpeed 40
#define namaBluetooth "Jws Basmalla"
#define DISPLAYS_ACROSS 2
#define DISPLAYS_DOWN 1

#define pinLampu 6
#define LAMPUON HIGH
#define LAMPUOFF LOW

// Object Declaration

Power power = Power();
Event second = Event(1000);
Event event300ms = Event(300);
Buzer myBuzer = Buzer(buzer);
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
Rtc myRtc = Rtc();
Segmen mySegmen = Segmen();
DFRobotDFPlayerMini myDFPlayer;
MyObject parameter; // Variable to store custom object read from EEPROM.
Jadwal jadwal = Jadwal();
//==================================
// Var Global
double times[sizeof(TimeName) / sizeof(char *)];
long timer;
volatile int alamat_eprom = 0;
int suruq;
unsigned char temp_min, temp_jam, temp_hri, rename_bt = 0; //, limaDetik;
unsigned char jam, menit, hari;

void ScanDMD()
{
  dmd.scanDisplayBySPI();
}
void setup()
{
  timer = millis();
  pinMode(tombol_up, INPUT_PULLUP);
  pinMode(tombol_menu, INPUT_PULLUP);
  pinMode(tombol_down, INPUT_PULLUP);
  pinMode(pinLampu, OUTPUT);
  Serial1.begin(bluetooth);
  Serial.begin(9600);
  Wire.begin();
  Serial1.setTimeout(200);
  dmd.clearScreen(true);
  Timer3.initialize(800); // period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
  Timer3.attachInterrupt(ScanDMD);
  for (unsigned char i = 0; i < 3; i++)
  {
    myBuzer.setOn();
    delay(250);
    myBuzer.setOff();
    delay(250);
  }
  myDFPlayer.begin(Serial);
  myDFPlayer.setTimeOut(500);
  set_calc_method(ISNA);
  set_asr_method(Shafii);
  set_high_lats_adjust_method(AngleBased);
  set_fajr_angle(20);
  set_isha_angle(18);
  dmd.selectFont(myFont);
  dmd.setBrightness(7);
  // if (digitalRead(tombol_up) == LOW)
  // {
  //   if (digitalRead(tombol_down) == LOW)
  //   {
  //     if (digitalRead(tombol_menu) == LOW)
  //     {
  //       reset();
  //     }
  //   }
  // }

  EEPROM.get(0, parameter);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  myDFPlayer.volume(25); // Set volume . From 0 to 30
  myDFPlayer.play(1);    // Play the first mp3
  display_eprom(text_run);
  power.setTimeOn(parameter.timeOn);
  power.setTimeOff(parameter.timeOff);

  power.setTimeOn(3 * 60);
  power.setTimeOff(22 * 60);
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
  // tombol();
  mySegmen.loop();
}
/***********************************************************************************/
void alarm(void)
{
  int waktu_alaram = myRtc.getIntTime();
  // lampu background on sebelum 15 menit
  for (uint8_t i = 0; i < 5; i++)
  {
    int dataCompare = data[waktu_subuh + i] - 15;
    if (waktu_alaram == dataCompare)
    {
      digitalWrite(pinLampu, LAMPUON); // hidupkan lapu
      break;
    }
  }
  // lampu background  off setelah 30 menit
  for (uint8_t i = 0; i < 5; i++)
  {
    int dataCompare = data[waktu_subuh + i] + 30;
    if (waktu_alaram == dataCompare)
    {
      digitalWrite(pinLampu, LAMPUOFF); // matikan lapu
      break;
    }
  }
  // play mp3 tilawah
  if (waktu_alaram == data[waktu_subuh] - 10)
  {
    myDFPlayer.playFolder(2, 1);
  }
  if (waktu_alaram == data[waktu_duhur] - 10)
  {
    myDFPlayer.playFolder(2, 2); // play  the mp3.
    // if (data[hari] == jumat)
    // else
  }
  if (waktu_alaram == data[waktu_ashar] - 10)
  {
    myDFPlayer.playFolder(2, 3);
  }
  if (waktu_alaram == data[waktu_magrib] - 10)
  {
    myDFPlayer.playFolder(2, 4);
  }
  if (waktu_alaram == data[waktu_isya] - 10)
  {
    myDFPlayer.playFolder(2, 5);
  }

  // Masuk waktu sholat
  if (waktu_alaram == data[waktu_imsya])
  {
    alarm_on(waktu_imsya);
  }
  if (waktu_alaram == suruq)
  {
    alarm_on(waktu_suruq);
  }
  if (waktu_alaram == data[waktu_subuh])
  {
    myDFPlayer.playFolder(1, 1); // play adzan
    alarm_on(waktu_subuh);
  }
  if (waktu_alaram == data[waktu_duhur])
  {
    myDFPlayer.playFolder(1, 2); // play adzan
    if (myRtc.getHari() == jumat)
    {
      alarm_on(waktu_jumat);
    }
    else
    {
      alarm_on(waktu_duhur);
    }
  }
  if (waktu_alaram == data[waktu_ashar])
  {
    myDFPlayer.playFolder(1, 3); // play adzan
    alarm_on(waktu_ashar);
  }
  if (waktu_alaram == data[waktu_magrib])
  {
    myDFPlayer.playFolder(1, 4); // play adzan
    alarm_on(waktu_magrib);
  }
  if (waktu_alaram == data[waktu_isya])
  {
    myDFPlayer.playFolder(1, 5); // play adzan
    alarm_on(waktu_isya);
  }
}

void alarm_on(uint8_t sholat)
{ // time out adzan
  uint8_t beep_alarm = 3;
  int time_adzan;
  int _alamat_text;
  int count_iqomah;
  int stanby_sholat;
  alamat_eprom = 0;
  // beep_alarm = parameter.beep; // parameter.beep;
  if (sholat == waktu_suruq)
  {
    time_adzan = 60;
    display_eprom(text_run);
    _alamat_text = text_run;
    mySegmen.displaySuruq();
  }
  if (sholat == waktu_imsya)
  {
    time_adzan = 60;
    display_eprom(text_run);
    _alamat_text = text_run;
    mySegmen.displayImsya();
  }
  if (sholat == waktu_subuh)
  {
    time_adzan = parameter.timer_adzan_subuh * 60;
    count_iqomah = 60 * parameter.iqomah_subuh;
    stanby_sholat = parameter.lama_sholat_subuh;
    display_eprom(text_iq_subuh);
    _alamat_text = text_iq_subuh;
    mySegmen.displaySubuh();
  }
  if (sholat == waktu_duhur)
  {
    time_adzan = parameter.timer_adzan_duhur * 60;
    count_iqomah = 60 * parameter.iqomah_duhur;
    stanby_sholat = parameter.lama_sholat_duhur;
    display_eprom(text_iq_duhur);
    _alamat_text = text_iq_duhur;
    mySegmen.displayDzuhur();
  }
  if (sholat == waktu_ashar)
  {
    time_adzan = parameter.timer_adzan_ashar * 60;
    count_iqomah = 60 * parameter.iqomah_ashar;
    stanby_sholat = parameter.lama_sholat_ashar;
    display_eprom(text_iq_ashar);
    _alamat_text = text_iq_ashar;
    mySegmen.displayAshar();
  }
  if (sholat == waktu_magrib)
  {
    time_adzan = parameter.timer_adzan_maghrib * 60;
    count_iqomah = 60 * parameter.iqomah_maghrib;
    stanby_sholat = parameter.lama_sholat_maghrib;
    display_eprom(text_iq_maghrib);
    _alamat_text = text_iq_maghrib;
    mySegmen.displayMaghrib();
  }
  if (sholat == waktu_isya)
  {
    time_adzan = parameter.timer_adzan_isya * 60;
    count_iqomah = 60 * parameter.iqomah_isya;
    stanby_sholat = parameter.lama_sholat_isya;
    display_eprom(text_iq_isya);
    _alamat_text = text_iq_isya;
    mySegmen.displayIsya();
  }
  if (sholat == waktu_jumat)
  {
    beep_alarm = 0; // parameter.beep;
    time_adzan = parameter.timer_adzan_jumat * 60;
    count_iqomah = 60 * parameter.iqomah_jumat;
    stanby_sholat = parameter.lama_sholat_jumat;
    display_eprom(text_iq_jumat);
    _alamat_text = text_iq_jumat;
    mySegmen.displayJumat();
  }
  time_adzan = 1 * 60;
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
  if (sholat == waktu_jumat)
  {
    return;
  }
  if (sholat == waktu_suruq)
  {
    return;
  }
  if (sholat == waktu_imsya)
  {
    return;
  }

  mySegmen.displayIqomah(); // tampil_hari(iqomah);
  myBuzer.onOnce(250);

  if (count_iqomah > 0)
  {
    count_iqomah = 30;
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
  stanby_sholat = stanby_sholat * 60;
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
  unsigned char wkt = pgm_read_byte_near(gmt + daerah);
  int hours, minutes;
  if (daerah == 0)
  {
    wkt = parameter.set_kota_gmt;
    lt = parameter.set_kota_lnt;
    bj = parameter.set_kota_bjr;
  }
  get_prayer_times(myRtc.getTahun(), myRtc.getBulan(), myRtc.getTanggal(), lt, bj, wkt, times);
  get_float_time_parts(times[0], hours, minutes);
  data[waktu_subuh] = (hours * 60) + minutes;

  get_float_time_parts(times[1], hours, minutes);
  // suruq = (hours * 60) + minutes + 15;
  suruq = (hours * 60) + minutes;
  get_float_time_parts(times[2], hours, minutes);
  data[waktu_duhur] = (hours * 60) + minutes;
  get_float_time_parts(times[3], hours, minutes);
  data[waktu_ashar] = (hours * 60) + minutes;
  get_float_time_parts(times[5], hours, minutes);
  data[waktu_magrib] = (hours * 60) + minutes;
  get_float_time_parts(times[6], hours, minutes);
  data[waktu_isya] = (hours * 60) + minutes;
  data[waktu_subuh] = data[waktu_subuh] + 2;
  data[waktu_duhur] = data[waktu_duhur] + 2;
  data[waktu_ashar] = data[waktu_ashar] + 2;
  data[waktu_magrib] = data[waktu_magrib] + 2;
  data[waktu_isya] = data[waktu_isya] + 2;

  data[waktu_subuh] = data[waktu_subuh] + parameter.tambah_kurang_subuh;
  data[waktu_duhur] = data[waktu_duhur] + parameter.tambah_kurang_duhur;
  data[waktu_ashar] = data[waktu_ashar] + parameter.tambah_kurang_ashar;
  data[waktu_magrib] = data[waktu_magrib] + parameter.tambah_kurang_maghrib;
  data[waktu_isya] = data[waktu_isya] + parameter.tambah_kurang_isya;
  if (parameter.jadwal_fix_subuh > 0)
    data[waktu_subuh] = parameter.jadwal_fix_subuh;
  if (parameter.jadwal_fix_duhur > 0)
    data[waktu_duhur] = parameter.jadwal_fix_duhur;
  if (parameter.jadwal_fix_ashar > 0)
    data[waktu_ashar] = parameter.jadwal_fix_ashar;
  if (parameter.jadwal_fix_maghrib > 0)
    data[waktu_magrib] = parameter.jadwal_fix_maghrib;
  if (parameter.jadwal_fix_isya > 0)
    data[waktu_isya] = parameter.jadwal_fix_isya;
  data[waktu_imsya] = data[waktu_subuh] - 10;

  // baca jadwal
  unsigned char jam, menit;
  jadwal.setOffsiteSubuh(parameter.tambah_kurang_subuh);
  jadwal.setOffsiteDzuhur(parameter.tambah_kurang_duhur);
  jadwal.setOffsiteAshar(parameter.tambah_kurang_ashar);
  jadwal.setOffsiteMaghrib(parameter.tambah_kurang_maghrib);
  jadwal.setOffsiteIsya(parameter.tambah_kurang_isya);
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
  if (serialEventRun)
  {
    serialEventRun();
  }
  if (millis() - timer >= runingTextSpeed)
  {
    if (dmd.stepMarquee(-1, 0))
    {
      display_eprom(_alamat_text);
    }
    timer = millis();
  }
}
//
const PROGMEM char bloothtCommand[] = {
    'J', // set jam ->2
    'S', // set stop mp3 ->3
    'I', // set iqomah ->4
    'T', // set tarhim ->5
    'B', // set brightnes ->6
    'F', // set offsite ->7
    'X', // set fix ->8
    'K', // set kota ->9
    'A', // set adzan
    'W', // play mp3 ->11
         // 'Y',//Power ->12
         // 'Z',//Buzer -> 13

};
//==================================================================================
void serialEvent1()
{
  String input_serial;
  if (Serial1.available())
  {
    input_serial = Serial1.readString();
    //    Serial.println(input_serial);
  }
  if (input_serial[0] == 'R')
  {
    if (input_serial[1] == 'E')
    {
      if (input_serial[2] == 'S')
      {
        if (input_serial[3] == 'E')
        {
          if (input_serial[4] == 'T')
          {
            reset();
            int loct = (input_serial[6] - '0') * 100;
            loct += (input_serial[7] - '0') * 10;
            loct += input_serial[8] - '0';
            if (loct < 317)
            {
              parameter.kota = loct;
              //          Serial.println(loct);
              EEPROM.put(0, parameter);
            }
            command = command_end;
          }
        }
      }
    }
  }
  if (input_serial == "1234")
  {
    Serial1.print("OK\n");
    command = command_start;
  }
  else
  {
    if (command == command_start)
    {
      if (input_serial[0] == '%')
      {
        for (uint8_t cmd = 0; cmd < 10; cmd++)
        {
          char lookupCmd = pgm_read_byte_near(bloothtCommand + cmd);
          if (lookupCmd == input_serial[1])
          {
            Serial1.print("OK");
            Serial1.print(lookupCmd);
            Serial1.print("\n");
            command = cmd + 2;

            break;
          }
        }
      }
    }
    //===========================================================
    else if (command == play_mp3)
    {
      command = command_end;
      int _track = ((input_serial[1] - '0') * 100) + ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
      // 1234 %W Q001
      // 1234 %W Q001
      // 1234 %W Q001
      // 1234 %W Q004
      // sholawat
      // 1234 %W W003
      // 1234 %W W010
      // stop
      // 1234 %W STOP
      // myDFPlayer.play(1);  //Play the first mp3
      // myDFPlayer.stop();

      if (input_serial[0] == 'Q')
      {
        myDFPlayer.play(_track); // play alquran
      }
      if (input_serial[0] == 'W')
      { // play sholawat
        myDFPlayer.play(_track + 114);
      }
      if (input_serial[0] == 'S')
      {
        myDFPlayer.stop(); // STOP
      }
      Serial1.print("SetPlay\n");
    }
    else if (command == set_jam)
    {
      command = command_end;
      // 290417040319 // DTK_MNT_JAM_TGL_BLN_TH
      unsigned char jam = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
      unsigned char menit = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
      unsigned char detik = ((input_serial[0] - '0') * 10) + (input_serial[1] - '0');
      unsigned char tanggal = ((input_serial[6] - '0') * 10) + (input_serial[7] - '0');
      unsigned char bulan = ((input_serial[8] - '0') * 10) + (input_serial[9] - '0');
      int tahun = 2000 + ((input_serial[10] - '0') * 10) + (input_serial[11] - '0');
      // tulis_rtc();
      myRtc.setJam(jam, menit, detik);
      myRtc.setTanggal(tanggal, bulan, tahun);
      Serial1.print("SetTime\n");
    }
    else if (command == set_text)
    { // ok

      command = command_end;
      // 1234 %S tes karakter
      // writeString(text_run,input_serial);
      int _size = input_serial.length();
      int i;
      for (i = 0; i < _size; i++)
        EEPROM.write(text_run + i, input_serial[i]);
      EEPROM.write(text_run + _size, '\0'); // Add termination null character for String Data
      Serial1.print("SetText\n");
      alamat_eprom = 0;
      display_eprom(text_run);
    }
    else if (command == set_iqomah)
    {
      // 1234 %I N 0 1010 Iqomah Subuh // n DAN y ADALAH ENABLE IQOMAH
      // 1234 %I N 1 1010 Iqomah Dzuhur... //IQOMAH_SHOLAT
      // 1234 %I N 2 1010 Iqomah Ashar...
      // 1234 %I N 3 1010 Iqomah Maghrib...
      // 1234 %I N 4 1010 Iqomah Isya...
      // 1234 %I N 5 0510 Iqomah jumat
      // 1234 %I Y 0 1010 Iqomah Subuh
      command = command_end;
      int _text_iqomah;
      if (input_serial[1] == '0')
      {
        parameter.iqomah_subuh = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
        parameter.lama_sholat_subuh = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
        // writeString(text_iq_subuh,input_serial.substring(6));
        _text_iqomah = text_iq_subuh;
      }
      if (input_serial[1] == '1')
      {
        parameter.iqomah_duhur = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
        parameter.lama_sholat_duhur = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
        // writeString(text_iq_duhur,input_serial.substring(6));
        _text_iqomah = text_iq_duhur;
      }
      if (input_serial[1] == '2')
      {
        parameter.iqomah_ashar = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
        parameter.lama_sholat_ashar = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
        // writeString(text_iq_ashar,input_serial.substring(6));
        _text_iqomah = text_iq_ashar;
      }
      if (input_serial[1] == '3')
      {
        parameter.iqomah_maghrib = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
        parameter.lama_sholat_maghrib = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
        // writeString(text_iq_maghrib,input_serial.substring(6));
        _text_iqomah = text_iq_maghrib;
      }
      if (input_serial[1] == '4')
      {
        parameter.iqomah_isya = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
        parameter.lama_sholat_isya = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
        // writeString(text_iq_isya,input_serial.substring(6));
        _text_iqomah = text_iq_isya;
      }
      if (input_serial[1] == '5')
      {
        parameter.iqomah_jumat = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
        parameter.lama_sholat_jumat = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
        // writeString(text_iq_jumat,input_serial.substring(6));
        _text_iqomah = text_iq_jumat;
      }
      int _size = input_serial.length();
      int i, u = 0;
      for (i = 6; i < _size; i++)
      {
        EEPROM.write(_text_iqomah + u, input_serial[i]);
        u++;
      }
      EEPROM.write(_text_iqomah + u, '\0');
      EEPROM.put(0, parameter);
      Serial1.print("SetIqom\n");
    }
    else if (command == set_tarhim)
    { // OK
      command = command_end;
      // 15 15 15 15 15 15 YNNNYY  //Subuh Dzuhur...  jumat YES/NO
      parameter.tartil_subuh = ((input_serial[0] - '0') * 10) + (input_serial[1] - '0');
      parameter.tartil_duhur = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
      parameter.tartil_ashar = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
      parameter.tartil_maghrib = ((input_serial[6] - '0') * 10) + (input_serial[7] - '0');
      parameter.tartil_isya = ((input_serial[8] - '0') * 10) + (input_serial[9] - '0');
      parameter.tartil_jumat = ((input_serial[10] - '0') * 10) + (input_serial[11] - '0');
      if (input_serial[12] == 'N')
        parameter.tartil_subuh = 0;
      if (input_serial[13] == 'N')
        parameter.tartil_duhur = 0; // ((_trtl[2]-'0')*10)+(_trtl[3]-'0');
      if (input_serial[14] == 'N')
        parameter.tartil_ashar = 0; // ((_trtl[4]-'0')*10)+(_trtl[5]-'0') ;
      if (input_serial[15] == 'N')
        parameter.tartil_maghrib = 0; //((_trtl[6]-'0')*10)+(_trtl[7]-'0');
      if (input_serial[16] == 'N')
        parameter.tartil_isya = 0; //((_trtl[8]-'0')*10)+(_trtl[9]-'0');
      if (input_serial[17] == 'N')
        parameter.tartil_jumat = 0; //((_trtl[10]-'0')*10)+(_trtl[11]-'0');
      EEPROM.put(0, parameter);
      Serial1.print("SetTrkm\n");
    }
    else if (command == set_brightnes)
    { // ok
      command = command_end;
      // 1234 %B 0300 01 0600 03 1730 01 2100 00
      // 1234 %B 0300 07 0600 03 1730 01 2100 00
      parameter.kecerahan_1 = input_serial[5] - '0';
      parameter.kecerahan_2 = input_serial[11] - '0';
      parameter.kecerahan_3 = input_serial[17] - '0';
      parameter.kecerahan_4 = input_serial[23] - '0';
      parameter.jam_kecerahan_1 = ((((input_serial[0] - '0') * 10) + (input_serial[1] - '0')) * 60) + (((input_serial[2] - '0') * 10) + (input_serial[3] - '0'));
      parameter.jam_kecerahan_2 = ((((input_serial[6] - '0') * 10) + (input_serial[7] - '0')) * 60) + (((input_serial[8] - '0') * 10) + (input_serial[9] - '0'));
      parameter.jam_kecerahan_3 = ((((input_serial[12] - '0') * 10) + (input_serial[13] - '0')) * 60) + (((input_serial[14] - '0') * 10) + (input_serial[15] - '0'));
      parameter.jam_kecerahan_4 = ((((input_serial[18] - '0') * 10) + (input_serial[19] - '0')) * 60) + (((input_serial[20] - '0') * 10) + (input_serial[21] - '0'));
      EEPROM.put(0, parameter);
      Serial1.print("SetBrns\n");
    }
    else if (command == set_offsite)
    { // OK
      command = command_end;
      // 1234 %F 19 02 01 03 05 //1= TAMBAH & 0=KURANG
      if (input_serial[0] == '0')
        parameter.tambah_kurang_subuh = 0 - (input_serial[1] - '0');
      else
        parameter.tambah_kurang_subuh = input_serial[1] - '0';
      if (input_serial[2] == '0')
        parameter.tambah_kurang_duhur = 0 - (input_serial[3] - '0');
      else
        parameter.tambah_kurang_duhur = input_serial[3] - '0';
      if (input_serial[4] == '0')
        parameter.tambah_kurang_ashar = 0 - (input_serial[5] - '0');
      else
        parameter.tambah_kurang_ashar = input_serial[5] - '0';
      if (input_serial[6] == '0')
        parameter.tambah_kurang_maghrib = 0 - (input_serial[7] - '0');
      else
        parameter.tambah_kurang_maghrib = input_serial[7] - '0';
      if (input_serial[8] == '0')
        parameter.tambah_kurang_isya = 0 - (input_serial[9] - '0');
      else
        parameter.tambah_kurang_isya = input_serial[9] - '0';
      EEPROM.put(0, parameter);
      Serial1.print("SetOffs\n");
    }
    else if (command == set_fix)
    { // OK
      command = command_end;
      // YYNYN 2200 0000 0000 0000 0000
      // 5 9 13 17 21
      parameter.jadwal_fix_subuh = 0;
      parameter.jadwal_fix_duhur = 0;
      parameter.jadwal_fix_ashar = 0;
      parameter.jadwal_fix_maghrib = 0;
      parameter.jadwal_fix_isya = 0;
      if (input_serial[0] == 'Y')
        parameter.jadwal_fix_subuh = ((((input_serial[5] - '0') * 10) + (input_serial[6] - '0')) * 60) + (((input_serial[7] - '0') * 10) + (input_serial[8] - '0'));
      if (input_serial[1] == 'Y')
        parameter.jadwal_fix_duhur = ((((input_serial[9] - '0') * 10) + (input_serial[10] - '0')) * 60) + (((input_serial[11] - '0') * 10) + (input_serial[12] - '0'));
      if (input_serial[2] == 'Y')
        parameter.jadwal_fix_ashar = ((((input_serial[13] - '0') * 10) + (input_serial[14] - '0')) * 60) + (((input_serial[15] - '0') * 10) + (input_serial[16] - '0'));
      if (input_serial[3] == 'Y')
        parameter.jadwal_fix_maghrib = ((((input_serial[17] - '0') * 10) + (input_serial[18] - '0')) * 60) + (((input_serial[19] - '0') * 10) + (input_serial[20] - '0'));
      if (input_serial[4] == 'Y')
        parameter.jadwal_fix_isya = ((((input_serial[21] - '0') * 10) + (input_serial[22] - '0')) * 60) + (((input_serial[23] - '0') * 10) + (input_serial[24] - '0'));
      EEPROM.put(0, parameter);
      Serial1.print("SetFixx\n");
    }
    else if (command == set_bt_kota)
    { // ok
      // EN 110 39 007 82 01187+00 //LU
      // ES 110 39 007 82 01187+00 //ls
      parameter.set_kota_bjr = ((input_serial[2] - '0') * 100) + ((input_serial[3] - '0') * 10) + (input_serial[4] - '0') + ((input_serial[5] - '0') * 0.1) + ((input_serial[6] - '0') * 0.01);
      if (input_serial[1] == 'N')
        parameter.set_kota_lnt = ((input_serial[7] - '0') * 100) + ((input_serial[8] - '0') * 10) + (input_serial[9] - '0') + ((input_serial[10] - '0') * 0.1) + ((input_serial[11] - '0') * 0.01);
      if (input_serial[1] == 'S')
        parameter.set_kota_lnt = 0 - (((input_serial[7] - '0') * 100) + ((input_serial[8] - '0') * 10) + (input_serial[9] - '0') + ((input_serial[10] - '0') * 0.1) + ((input_serial[11] - '0') * 0.01));
      parameter.set_kota_gmt = input_serial[16] - '0';
      parameter.kota = 0;
      EEPROM.put(0, parameter);
      Serial1.print("SetKoor\n");
      command = command_end;
    }
    else if (command == set_adzan)
    { // OK
      command = command_end;
      parameter.timer_adzan_subuh = ((input_serial[0] - '0') * 10) + (input_serial[1] - '0');
      parameter.timer_adzan_duhur = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
      parameter.timer_adzan_ashar = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
      parameter.timer_adzan_maghrib = ((input_serial[6] - '0') * 10) + (input_serial[7] - '0');
      parameter.timer_adzan_isya = ((input_serial[8] - '0') * 10) + (input_serial[9] - '0');
      parameter.timer_adzan_jumat = ((input_serial[10] - '0') * 10) + (input_serial[11] - '0');
      EEPROM.put(0, parameter);
      Serial1.print("SetAlrm\n");
    }
  }

  //===========================================================
  if (command == command_end)
  {
    myBuzer.setOn();
    baca_jadwal(parameter.kota);
    myBuzer.setOff();
    command = 0;
  }
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
  parameter.kota = 0;
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
  parameter.timeOn = 3;
  parameter.timeOff = 22;
  EEPROM.put(0, parameter);

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
void tunggu_menu()
{
  myBuzer.onOnce(100);
  while (digitalRead(tombol_menu) == LOW)
  {
    myBuzer.loop();
    delay(150);
  }
}
void tunggu_up()
{
  int skip = 500;

  myBuzer.onOnce(100);
  while (digitalRead(tombol_up) == LOW)
  {
    myBuzer.loop();
    mySegmen.loop();
    tampil_text(text_run);
    if (--skip == 0)
      break;
  }
}
void tunggu_down()
{
  int skip = 500;
  myBuzer.onOnce(100);
  while (digitalRead(tombol_down) == LOW)
  {
    myBuzer.loop();
    mySegmen.loop();
    tampil_text(text_run);
    if (--skip == 0)
      break;
  }
}
// seting(data,bawah,atas)
// void set_waktu(byte a, int b, int c)
// {
//   for (byte l = 12; l < 36; l++)
//     segmen[l] = (data_jadwal[10]);
//   while (digitalRead(tombol_menu) == 1)
//   {
//     if (digitalRead(tombol_up) == 0)
//     {
//       data[a]++;
//       if (data[a] > c)
//         data[a] = b;
//       refres(jam);
//       refres(tanggal);
//       tunggu_up();
//     }
//     if (digitalRead(tombol_down) == 0)
//     {
//       data[a]--;
//       if (data[a] < b || data[a] == 0xff)
//         data[a] = c;
//       refres(jam);
//       refres(tanggal);
//       tunggu_down();
//     }
//     refres(jam);
//     refres(tanggal);
//     if ((millis() % 300) < 150)
//     {
//       switch (a)
//       {
//       case menit:
//         segmen[2] = data_jam[10];
//         segmen[3] = data_jam[10];
//         break;
//       case jam:
//         segmen[0] = data_jam[10];
//         segmen[1] = data_jam[10];
//         break;
//       case tanggal:
//         segmen[4] = data_kalender[10];
//         segmen[5] = data_kalender[10];
//         break;
//       case bulan:
//         segmen[7] = data_kalender[10];
//         segmen[8] = data_kalender[10];
//         break;
//       case tahun:
//         segmen[10] = data_kalender[10];
//         segmen[11] = data_kalender[10];
//       }
//     }
//     mySegmen.loop();
//     tampil_text(text_run);
//   }
//   tunggu_menu();
// }
// // seting (nama,val,bawah,atas)
// int set_parameter(byte _nama_set, int _nilai, int _bawah, int _atas)
// {
//   for (byte l = 12; l < 36; l++)
//     segmen[l] = (data_jadwal[10]);
//   tampil_hari(_nama_set);
//   while (digitalRead(tombol_menu) == 1)
//   {
//     refres(jam);
//     if (_nilai > 99)
//     {
//       data[menit] = _nilai % 100;
//       data[jam] = _nilai / 100;
//     }
//     else
//     {
//       segmen[0] = (data_jam[10]);
//       segmen[1] = (data_jam[10]);
//       if (_nilai < 0)
//         segmen[1] = 255 - 128;
//       data[menit] = abs(_nilai);
//     }
//     if (digitalRead(tombol_up) == 0)
//     {
//       _nilai++;
//       if (_nilai > _atas)
//         _nilai = _bawah;
//       refres(jam);
//       if (_nilai > 99)
//       {
//         data[menit] = _nilai % 100;
//         data[jam] = _nilai / 100;
//       }
//       else
//       {
//         segmen[0] = (data_jam[10]);
//         segmen[1] = (data_jam[10]);
//         if (_nilai < 0)
//           segmen[1] = 255 - 128;
//         data[menit] = abs(_nilai);
//       }
//       tunggu_up();
//     }
//     if (digitalRead(tombol_down) == 0)
//     {
//       _nilai--;
//       if (_nilai < _bawah)
//         _nilai = _atas;
//       refres(jam);
//       if (_nilai > 99)
//       {
//         data[menit] = _nilai % 100;
//         data[jam] = _nilai / 100;
//       }
//       else
//       {
//         segmen[0] = (data_jam[10]);
//         segmen[1] = (data_jam[10]);
//         if (_nilai < 0)
//           segmen[1] = 255 - 128;
//         data[menit] = abs(_nilai);
//       }
//       tunggu_down();
//     }
//     if ((millis() % 300) < 150)
//     {
//       segmen[0] = data_jam[10];
//       segmen[1] = data_jam[10];
//       segmen[2] = data_jam[10];
//       segmen[3] = data_jam[10];
//     }
//     mySegmen.loop();
//     tampil_text(text_run);
//   }
//   tunggu_menu();
//   return _nilai;
// }
// void tombol()
// {
//   unsigned char a;
//   if (digitalRead(tombol_menu) == LOW)
//   {
//     tunggu_menu();
//     set_waktu(2, 0, 23);      // jam
//     set_waktu(1, 0, 59);      // menit
//     set_waktu(4, 1, 31);      // tanggal
//     set_waktu(5, 1, 12);      // bulan
//     set_waktu(6, 2000, 2099); // tahun
//     tulis_rtc();
//     baca_jadwal(parameter.kota);
//     refres(all);
//   }
//   if (digitalRead(tombol_up) == LOW)
//   {
//     tunggu_up();
//     parameter.timer_adzan_subuh = 3;
//     parameter.timer_adzan_subuh = 3;
//     parameter.timer_adzan_duhur = 3;
//     parameter.timer_adzan_ashar = 3;
//     parameter.timer_adzan_maghrib = 3;
//     parameter.timer_adzan_isya = 3;
//     parameter.timer_adzan_jumat = 3;
//     parameter.iqomah_subuh = set_parameter(subuh, parameter.iqomah_subuh, 0, 99);
//     parameter.iqomah_duhur = set_parameter(duhur, parameter.iqomah_duhur, 0, 99);
//     parameter.iqomah_ashar = set_parameter(ashar, parameter.iqomah_ashar, 0, 99);
//     parameter.iqomah_maghrib = set_parameter(magrib, parameter.iqomah_maghrib, 0, 99);
//     parameter.iqomah_isya = set_parameter(isya, parameter.iqomah_isya, 0, 99);
//     parameter.iqomah_jumat = set_parameter(kutbah, parameter.iqomah_jumat, 0, 99);
//     parameter.lama_sholat_subuh = set_parameter(stanbay, parameter.lama_sholat_subuh, 0, 99);
//     parameter.lama_sholat_duhur = parameter.lama_sholat_subuh;
//     parameter.lama_sholat_ashar = parameter.lama_sholat_subuh;
//     parameter.lama_sholat_maghrib = parameter.lama_sholat_subuh;
//     parameter.lama_sholat_isya = parameter.lama_sholat_subuh;
//     parameter.lama_sholat_jumat = parameter.lama_sholat_subuh;
//     parameter.beep = set_parameter(alarm_beep, parameter.beep, 0, 99);
//     parameter.jam_kecerahan_4 = 60 * set_parameter(off, parameter.jam_kecerahan_4 / 60, 0, 23);
//     parameter.jam_kecerahan_1 = 60 * set_parameter(on, parameter.jam_kecerahan_1 / 60, 0, 23);
//     parameter.jam_kecerahan_2 = parameter.jam_kecerahan_1;
//     parameter.jam_kecerahan_3 = parameter.jam_kecerahan_1;
//     parameter.kecerahan_1 = 7;
//     parameter.kecerahan_2 = 7;
//     parameter.kecerahan_3 = 7;
//     parameter.kecerahan_4 = 0;
//     parameter.jadwal_fix_subuh = 0;
//     parameter.jadwal_fix_duhur = 0;
//     parameter.jadwal_fix_ashar = 0;
//     parameter.jadwal_fix_maghrib = 0;
//     parameter.jadwal_fix_isya = 0;
//     parameter.jadwal_fix_subuh = 0;
//     parameter.jadwal_fix_duhur = 0;
//     parameter.jadwal_fix_ashar = 0;
//     parameter.jadwal_fix_maghrib = 0;
//     parameter.jadwal_fix_isya = 0;
//     EEPROM.put(0, parameter);
//     baca_jadwal(parameter.kota);
//     refres(all);
//   }
//   if (digitalRead(tombol_down) == LOW)
//   {
//     a = 0;
//     while (digitalRead(tombol_down) == 0)
//     {
//       delay(100);
//       a++;
//       if (a == 50)
//       {
//         tunggu_down();
//         while (digitalRead(tombol_down) == LOW)
//           ;
//         delay(500);
//         parameter.kota = set_parameter(set_kota, parameter.kota, 0, 316);
//         parameter.tambah_kurang_subuh = set_parameter(subuh, parameter.tambah_kurang_subuh, -10, 10);
//         parameter.tambah_kurang_duhur = set_parameter(duhur, parameter.tambah_kurang_duhur, -10, 10);      // parameter.tambah_kurang_subuh;
//         parameter.tambah_kurang_ashar = set_parameter(ashar, parameter.tambah_kurang_ashar, -10, 10);      // parameter.tambah_kurang_subuh;
//         parameter.tambah_kurang_maghrib = set_parameter(magrib, parameter.tambah_kurang_maghrib, -10, 10); // parameter.tambah_kurang_subuh;
//         parameter.tambah_kurang_isya = set_parameter(isya, parameter.tambah_kurang_isya, -10, 10);         // parameter.tambah_kurang_subuh;
//         parameter.tartil_subuh = set_parameter(tlwh_1, parameter.tartil_subuh, 0, 99);
//         parameter.tartil_duhur = set_parameter(tlwh_2, parameter.tartil_duhur, 0, 99);
//         parameter.tartil_ashar = set_parameter(tlwh_3, parameter.tartil_ashar, 0, 99);
//         parameter.tartil_maghrib = set_parameter(tlwh_4, parameter.tartil_maghrib, 0, 99);
//         parameter.tartil_isya = set_parameter(tlwh_5, parameter.tartil_isya, 0, 99);
//         parameter.tartil_jumat = set_parameter(tlwh_6, parameter.tartil_jumat, 0, 99);
//         EEPROM.put(0, parameter);
//         baca_jadwal(parameter.kota);
//         refres(all);
//       }
//     }
//   }
// }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
