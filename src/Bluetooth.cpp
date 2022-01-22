

#include "Bluetooth.h"
void Bluetooth::setEprom(EpromObject *param)
{
  parameter = param;
}

void Bluetooth::setPower(Power *power)
{
  this->power = power;
}

void Bluetooth::setBuzer(Buzer *buzer)
{
  this->buzer = buzer;
}
void Bluetooth::setRtc(Rtc *rtc)
{
  this->rtc = rtc;
}
// 1 = Read jadwal
// 2 = Refres Runing text
// 3 = Reset Default;
unsigned char Bluetooth::getStatus()
{
  unsigned char give = status;
  status = 0;
  return give;
}
unsigned char Bluetooth::available()
{
  return status;
}
void Bluetooth::loop()
{
  char bloothtCommand[] = {
      'J', // set jam ->2
      'S', // set stop mp3 ->3
      'I', // set iqomah ->4
      'T', // set tarhim ->5
      'B', // set brightnes ->6
      'F', // set offsite ->7
      'X', // set fix ->8
      'K', // set kota ->9
      'A', // set adzan ->10
      'W', // play mp3 ->11
      'Y', // Power ->12
      'Z', // Buzer -> 13
      'R', // Reset Pabrik -> 14
  };
  String input_serial;
  if (Serial1.available())
  {
    input_serial = Serial1.readString();
    // Serial.print(input_serial);
  }

  if (input_serial == "1234")
  {
    Serial1.print("OK\n");
    command = command_start;
    return;
  }
  else
  {
    if (command == command_start)
    {
      if (input_serial[0] == '%')
      {
        for (uint8_t cmd = 0; cmd < 13; cmd++)
        {
          char lookupCmd = bloothtCommand[cmd];
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
    //===========================================
    else if (command == RESET_PABRIK)
    {
      command = command_end;
      // 1234 %R 123 ->lokasi
      int lokasi = ((input_serial[0] - '0') * 100) + ((input_serial[1] - '0') * 10) + (input_serial[2] - '0');
      parameter->kota = lokasi;
      EEPROM.put(0, parameter);
      status = 3;
      Serial1.print("Reset\n");
    }
    //===========================================
    else if (command == SET_BUZER)
    {
      command = command_end;
      // 1234 %Y 12
      unsigned char bzr = ((input_serial[0] - '0') * 10) + (input_serial[1] - '0');
      parameter->beep = bzr;
      EEPROM.put(0, parameter);
      Serial1.print("SetBuzer\n");
    }
    //===========================================
    else if (command == SET_POWER)
    {
      command = command_end;
      // 1234 %Y 0300 2230  - > on off
      int jam = ((input_serial[0] - '0') * 10) + (input_serial[1] - '0');
      int menit = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
      int on = (jam * 60) + menit;
      parameter->timeOn = on;
      jam = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
      menit = ((input_serial[6] - '0') * 10) + (input_serial[7] - '0');
      int off = (jam * 60) + menit;
      parameter->timeOff = off;
      power->setTimeOn(on);
      power->setTimeOff(off);
      EEPROM.put(0, parameter);
      Serial1.print("SetPower\n");
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
        myPlayer->play(_track); // play alquran
      }
      if (input_serial[0] == 'W')
      { // play sholawat
        myPlayer->play(_track + 114);
      }
      if (input_serial[0] == 'S')
      {
        myPlayer->stop(); // STOP
      }
      Serial1.print("SetPlay\n");
    }
    else if (command == set_jam)
    {
      status = 1;
      command = command_end;
      // 290417040319 // DTK_MNT_JAM_TGL_BLN_TH
      unsigned char jam = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
      unsigned char menit = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
      unsigned char detik = ((input_serial[0] - '0') * 10) + (input_serial[1] - '0');
      unsigned char tanggal = ((input_serial[6] - '0') * 10) + (input_serial[7] - '0');
      unsigned char bulan = ((input_serial[8] - '0') * 10) + (input_serial[9] - '0');
      int tahun = 2000 + ((input_serial[10] - '0') * 10) + (input_serial[11] - '0');
      // tulis_rtc();
      rtc->setJam(jam, menit, detik);
      rtc->setTanggal(tanggal, bulan, tahun);
      Serial1.print("SetTime\n");
    }
    else if (command == set_text)
    { // ok

      status = 2;
      command = command_end;
      // 1234 %S tes karakter
      // writeString(text_run,input_serial);
      int _size = input_serial.length();
      int i;
      for (i = 0; i < _size; i++)
      {

        EEPROM.write(TEXT_RUN + i, input_serial[i]);
      }
      EEPROM.write(TEXT_RUN + _size, '\0'); // Add termination null character for String Data
      Serial1.print("SetText\n");
      // alamat_eprom = 0;
      // display_eprom(text_run);
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
        parameter->iqomah_subuh = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
        parameter->lama_sholat_subuh = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
        // writeString(text_iq_subuh,input_serial.substring(6));
        _text_iqomah = TEXT_IQ_SUBUH; // text_iq_subuh;
      }
      if (input_serial[1] == '1')
      {
        parameter->iqomah_duhur = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
        parameter->lama_sholat_duhur = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
        // writeString(text_iq_duhur,input_serial.substring(6));
        _text_iqomah = TEXT_IQ_DUHUR; // text_iq_duhur;
      }
      if (input_serial[1] == '2')
      {
        parameter->iqomah_ashar = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
        parameter->lama_sholat_ashar = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
        // writeString(text_iq_ashar,input_serial.substring(6));
        _text_iqomah = TEXT_IQ_ASHAR; // text_iq_ashar;
      }
      if (input_serial[1] == '3')
      {
        parameter->iqomah_maghrib = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
        parameter->lama_sholat_maghrib = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
        // writeString(text_iq_maghrib,input_serial.substring(6));
        _text_iqomah = TEXT_IQ_MAGHRIB; // text_iq_maghrib;
      }
      if (input_serial[1] == '4')
      {
        parameter->iqomah_isya = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
        parameter->lama_sholat_isya = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
        // writeString(text_iq_isya,input_serial.substring(6));
        _text_iqomah = TEXT_IQ_ISYA; // text_iq_isya;
      }
      if (input_serial[1] == '5')
      {
        parameter->iqomah_jumat = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
        parameter->lama_sholat_jumat = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
        // writeString(text_iq_jumat,input_serial.substring(6));
        _text_iqomah = TEXT_IQ_JUMAT; // text_iq_jumat;
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
      parameter->tartil_subuh = ((input_serial[0] - '0') * 10) + (input_serial[1] - '0');
      parameter->tartil_duhur = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
      parameter->tartil_ashar = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
      parameter->tartil_maghrib = ((input_serial[6] - '0') * 10) + (input_serial[7] - '0');
      parameter->tartil_isya = ((input_serial[8] - '0') * 10) + (input_serial[9] - '0');
      parameter->tartil_jumat = ((input_serial[10] - '0') * 10) + (input_serial[11] - '0');
      if (input_serial[12] == 'N')
        parameter->tartil_subuh = 0;
      if (input_serial[13] == 'N')
        parameter->tartil_duhur = 0; // ((_trtl[2]-'0')*10)+(_trtl[3]-'0');
      if (input_serial[14] == 'N')
        parameter->tartil_ashar = 0; // ((_trtl[4]-'0')*10)+(_trtl[5]-'0') ;
      if (input_serial[15] == 'N')
        parameter->tartil_maghrib = 0; //((_trtl[6]-'0')*10)+(_trtl[7]-'0');
      if (input_serial[16] == 'N')
        parameter->tartil_isya = 0; //((_trtl[8]-'0')*10)+(_trtl[9]-'0');
      if (input_serial[17] == 'N')
        parameter->tartil_jumat = 0; //((_trtl[10]-'0')*10)+(_trtl[11]-'0');
      EEPROM.put(0, parameter);
      Serial1.print("SetTrkm\n");
    }
    else if (command == set_brightnes)
    { // ok
      command = command_end;
      // 1234 %B 0300 01 0600 03 1730 01 2100 00
      // 1234 %B 0300 07 0600 03 1730 01 2100 00
      parameter->kecerahan_1 = input_serial[5] - '0';
      parameter->kecerahan_2 = input_serial[11] - '0';
      parameter->kecerahan_3 = input_serial[17] - '0';
      parameter->kecerahan_4 = input_serial[23] - '0';
      parameter->jam_kecerahan_1 = ((((input_serial[0] - '0') * 10) + (input_serial[1] - '0')) * 60) + (((input_serial[2] - '0') * 10) + (input_serial[3] - '0'));
      parameter->jam_kecerahan_2 = ((((input_serial[6] - '0') * 10) + (input_serial[7] - '0')) * 60) + (((input_serial[8] - '0') * 10) + (input_serial[9] - '0'));
      parameter->jam_kecerahan_3 = ((((input_serial[12] - '0') * 10) + (input_serial[13] - '0')) * 60) + (((input_serial[14] - '0') * 10) + (input_serial[15] - '0'));
      parameter->jam_kecerahan_4 = ((((input_serial[18] - '0') * 10) + (input_serial[19] - '0')) * 60) + (((input_serial[20] - '0') * 10) + (input_serial[21] - '0'));
      EEPROM.put(0, parameter);
      Serial1.print("SetBrns\n");
    }
    else if (command == set_offsite)
    { // OK
      command = command_end;
      status = 1;
      // 1234 %F 19 02 01 03 05 //1= TAMBAH & 0=KURANG
      if (input_serial[0] == '0')
        parameter->tambah_kurang_subuh = 0 - (input_serial[1] - '0');
      else
        parameter->tambah_kurang_subuh = input_serial[1] - '0';
      if (input_serial[2] == '0')
        parameter->tambah_kurang_duhur = 0 - (input_serial[3] - '0');
      else
        parameter->tambah_kurang_duhur = input_serial[3] - '0';
      if (input_serial[4] == '0')
        parameter->tambah_kurang_ashar = 0 - (input_serial[5] - '0');
      else
        parameter->tambah_kurang_ashar = input_serial[5] - '0';
      if (input_serial[6] == '0')
        parameter->tambah_kurang_maghrib = 0 - (input_serial[7] - '0');
      else
        parameter->tambah_kurang_maghrib = input_serial[7] - '0';
      if (input_serial[8] == '0')
        parameter->tambah_kurang_isya = 0 - (input_serial[9] - '0');
      else
        parameter->tambah_kurang_isya = input_serial[9] - '0';
      EEPROM.put(0, parameter);
      Serial1.print("SetOffs\n");
    }
    else if (command == set_fix)
    { // OK
      command = command_end;
      status = 1;
      // YYNYN 2200 0000 0000 0000 0000
      // 5 9 13 17 21
      parameter->jadwal_fix_subuh = 0;
      parameter->jadwal_fix_duhur = 0;
      parameter->jadwal_fix_ashar = 0;
      parameter->jadwal_fix_maghrib = 0;
      parameter->jadwal_fix_isya = 0;
      if (input_serial[0] == 'Y')
        parameter->jadwal_fix_subuh = ((((input_serial[5] - '0') * 10) + (input_serial[6] - '0')) * 60) + (((input_serial[7] - '0') * 10) + (input_serial[8] - '0'));
      if (input_serial[1] == 'Y')
        parameter->jadwal_fix_duhur = ((((input_serial[9] - '0') * 10) + (input_serial[10] - '0')) * 60) + (((input_serial[11] - '0') * 10) + (input_serial[12] - '0'));
      if (input_serial[2] == 'Y')
        parameter->jadwal_fix_ashar = ((((input_serial[13] - '0') * 10) + (input_serial[14] - '0')) * 60) + (((input_serial[15] - '0') * 10) + (input_serial[16] - '0'));
      if (input_serial[3] == 'Y')
        parameter->jadwal_fix_maghrib = ((((input_serial[17] - '0') * 10) + (input_serial[18] - '0')) * 60) + (((input_serial[19] - '0') * 10) + (input_serial[20] - '0'));
      if (input_serial[4] == 'Y')
        parameter->jadwal_fix_isya = ((((input_serial[21] - '0') * 10) + (input_serial[22] - '0')) * 60) + (((input_serial[23] - '0') * 10) + (input_serial[24] - '0'));
      EEPROM.put(0, parameter);
      Serial1.print("SetFixx\n");
    }
    else if (command == set_bt_kota)
    { // ok
      command = command_end;

      status = 1;
      // EN 110 39 007 82 01187+00 //LU
      // ES 110 39 007 82 01187+00 //ls

      parameter->set_kota_bjr = ((input_serial[2] - '0') * 100) + ((input_serial[3] - '0') * 10) + (input_serial[4] - '0') + ((input_serial[5] - '0') * 0.1) + ((input_serial[6] - '0') * 0.01);
      if (input_serial[1] == 'N')
        parameter->set_kota_lnt = ((input_serial[7] - '0') * 100) + ((input_serial[8] - '0') * 10) + (input_serial[9] - '0') + ((input_serial[10] - '0') * 0.1) + ((input_serial[11] - '0') * 0.01);
      if (input_serial[1] == 'S')
        parameter->set_kota_lnt = 0 - (((input_serial[7] - '0') * 100) + ((input_serial[8] - '0') * 10) + (input_serial[9] - '0') + ((input_serial[10] - '0') * 0.1) + ((input_serial[11] - '0') * 0.01));
      parameter->set_kota_gmt = input_serial[16] - '0';
      parameter->kota = 0;
      EEPROM.put(0, parameter);
      Serial1.print("SetKoor\n");
    }
    else if (command == set_adzan)
    { // OK
      command = command_end;
      parameter->timer_adzan_subuh = ((input_serial[0] - '0') * 10) + (input_serial[1] - '0');
      parameter->timer_adzan_duhur = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
      parameter->timer_adzan_ashar = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
      parameter->timer_adzan_maghrib = ((input_serial[6] - '0') * 10) + (input_serial[7] - '0');
      parameter->timer_adzan_isya = ((input_serial[8] - '0') * 10) + (input_serial[9] - '0');
      parameter->timer_adzan_jumat = ((input_serial[10] - '0') * 10) + (input_serial[11] - '0');
      EEPROM.put(0, parameter);
      Serial1.print("SetAlrm\n");
    }
  }
  //===========================================================
  if (command == command_end)
  {

    buzer->onOnce(150);
    command = 0;
  }
}
Bluetooth::Bluetooth()
{
}

Bluetooth::~Bluetooth()
{
}
