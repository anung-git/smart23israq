#include "Bluetooth.h"
void Bluetooth::setPlayer(DFRobotDFPlayerMini *player)
{
  myPlayer = player;
}
void Bluetooth::setEprom(Memory *e)
{

  this->eprom = e;
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
void Bluetooth::playManual(String &data)
{
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
  command = command_end;
  unsigned char _track = ((data[1] - '0') * 100) + ((data[2] - '0') * 10) + (data[3] - '0');
  unsigned char volume = (data[1] - '0') * 10 + (data[2] - '0');
  switch (data[0])
  {
  case 'Q':
    myPlayer->playFolder(2, _track); // play alquran
    break;
  case 'T':
    myPlayer->volume(volume);
    eprom->setVolumemp3(volume);
    eprom->setEqualizer(data[3] - '0');
    myPlayer->EQ(data[3] - '0');
    break;
  case 'W':
    myPlayer->playFolder(1, data[1] - '0'); // play adzan
    break;
  }

  if (data == "STOP")
  {
    myPlayer->stop(); // STOP
  }
  Serial1.print("SetPlay\n");
  // Serial.print("volume = ");
  // Serial.println(PARAMETER.volumeMp3);
  // Serial.print("EQ = ");
  // Serial.println(PARAMETER.equalizer);
  // Serial.print("addr volume mp3 = ");
  // Serial.println(int(&PARAMETER));
}

void Bluetooth::setPower(String &data)
{
  command = command_end;
  // 1234 %Y 0300 2230  - > on off
  int jam = ((data[0] - '0') * 10) + (data[1] - '0');
  int menit = ((data[2] - '0') * 10) + (data[3] - '0');
  int on = (jam * 60) + menit;
  eprom->setTimeOn(on);
  jam = ((data[4] - '0') * 10) + (data[5] - '0');
  menit = ((data[6] - '0') * 10) + (data[7] - '0');
  int off = (jam * 60) + menit;
  eprom->setTimeOff(off);
  power->setTimeOn(on);
  power->setTimeOff(off);
  Serial1.print("SetPower\n");
}
void Bluetooth::setTime(String &data)
{
  status = 1;
  command = command_end;
  // 290417040319 // DTK_MNT_JAM_TGL_BLN_TH
  unsigned char jam = ((data[4] - '0') * 10) + (data[5] - '0');
  unsigned char menit = ((data[2] - '0') * 10) + (data[3] - '0');
  unsigned char detik = ((data[0] - '0') * 10) + (data[1] - '0');
  unsigned char tanggal = ((data[6] - '0') * 10) + (data[7] - '0');
  unsigned char bulan = ((data[8] - '0') * 10) + (data[9] - '0');
  int tahun = 2000 + ((data[10] - '0') * 10) + (data[11] - '0');
  // tulis_rtc();
  rtc->setJam(jam, menit, detik);
  rtc->setTanggal(tanggal, bulan, tahun);
  Serial1.print("SetTime\n");
}

void Bluetooth::setIqomah(String &data)
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
  unsigned char iqomah, sholat;
  if (data[1] == '0')
  {
    iqomah = ((data[2] - '0') * 10) + (data[3] - '0');
    sholat = ((data[4] - '0') * 10) + (data[5] - '0');
    eprom->setIqomahSubuh(iqomah);
    eprom->setLamaSholatSubuh(sholat);
    // writeString(text_iq_subuh,data.substring(6));
    _text_iqomah = TEXT_IQ_SUBUH; // text_iq_subuh;
  }
  if (data[1] == '1')
  {
    iqomah = ((data[2] - '0') * 10) + (data[3] - '0');
    sholat = ((data[4] - '0') * 10) + (data[5] - '0');
    eprom->setIqomahDuhur(iqomah);
    eprom->setLamaSholatDuhur(sholat);
    // writeString(text_iq_duhur,data.substring(6));
    _text_iqomah = TEXT_IQ_DUHUR; // text_iq_duhur;
  }
  if (data[1] == '2')
  {
    iqomah = ((data[2] - '0') * 10) + (data[3] - '0');
    sholat = ((data[4] - '0') * 10) + (data[5] - '0');
    eprom->setIqomahAshar(iqomah);
    eprom->setLamaSholatAshar(sholat);
    // writeString(text_iq_ashar,data.substring(6));
    _text_iqomah = TEXT_IQ_ASHAR; // text_iq_ashar;
  }
  if (data[1] == '3')
  {
    iqomah = ((data[2] - '0') * 10) + (data[3] - '0');
    sholat = ((data[4] - '0') * 10) + (data[5] - '0');
    eprom->setIqomahMaghrib(iqomah);
    eprom->setLamaSholatMaghrib(sholat);
    // writeString(text_iq_maghrib,data.substring(6));
    _text_iqomah = TEXT_IQ_MAGHRIB; // text_iq_maghrib;
  }
  if (data[1] == '4')
  {
    iqomah = ((data[2] - '0') * 10) + (data[3] - '0');
    sholat = ((data[4] - '0') * 10) + (data[5] - '0');
    eprom->setIqomahIsya(iqomah);
    eprom->setLamaSholatIsya(sholat);
    // writeString(text_iq_isya,data.substring(6));
    _text_iqomah = TEXT_IQ_ISYA; // text_iq_isya;
  }
  if (data[1] == '5')
  {
    iqomah = ((data[2] - '0') * 10) + (data[3] - '0');
    sholat = ((data[4] - '0') * 10) + (data[5] - '0');
    eprom->setIqomahJumat(iqomah);
    eprom->setLamaSholatJumat(sholat);
    // writeString(text_iq_jumat,data.substring(6));
    _text_iqomah = TEXT_IQ_JUMAT; // text_iq_jumat;
  }
  int _size = data.length();
  int i, u = 0;
  for (i = 6; i < _size; i++)
  {
    EEPROM.write(_text_iqomah + u, data[i]);
    u++;
  }
  EEPROM.write(_text_iqomah + u, '\0');
  // EEPROM.put(1, PARAMETER);
  Serial1.print("SetIqom\n");
}
void Bluetooth::setAdzan(String &data)
{
  command = command_end;
  eprom->setTimerAdzanSubuh(((data[0] - '0') * 10) + (data[1] - '0'));
  eprom->setTimerAdzanDuhur(((data[2] - '0') * 10) + (data[3] - '0'));
  eprom->setIndexAdzanAshar(((data[4] - '0') * 10) + (data[5] - '0'));
  eprom->setTimerAdzanMaghrib(((data[6] - '0') * 10) + (data[7] - '0'));
  eprom->setTimerAdzanIsya(((data[8] - '0') * 10) + (data[9] - '0'));
  eprom->setTimerAdzanJumat(((data[10] - '0') * 10) + (data[11] - '0'));
  Serial1.print("SetAlrm\n");
}
void Bluetooth::setTarhim(String &data)
{
  int surah = ((data[1] - '0') * 100) + ((data[2] - '0') * 10) + (data[3] - '0');
  unsigned char lamaTilawah = ((data[4] - '0') * 10) + (data[5] - '0');
  unsigned char adzan = (data[6] - '0');
  command = command_end;
  //  surah = 000
  //  lamaTilawah = 10
  //  adzan = 1
  //  0 000 10 1
  // Serial.print("Surah: ");
  // Serial.println(surah);
  // Serial.print(" Lama: ");
  // Serial.println(lamaTilawah);
  // Serial.print(" Adzan: ");
  // Serial.println(adzan);
  switch (data[0] - '0')
  {
  case 0:
    eprom->setIndexTilawahSubuh(surah);
    eprom->setTimeTilawahSubuh(lamaTilawah);
    eprom->setIndexAdzanSubuh(adzan);
    break;
  case 1:
    eprom->setIndexTilawahDzuhur(surah);
    eprom->setTimeTilawahDzuhur(lamaTilawah);
    eprom->setIndexAdzanDzuhur(adzan);
    break;
  case 2:
    eprom->setIndexTilawahAshar(surah);
    eprom->setTimeTilawahAshar(lamaTilawah);
    eprom->setIndexAdzanAshar(adzan);
    break;
  case 3:
    eprom->setIndexTilawahMaghrib(surah);
    eprom->setTimeTilawahMaghrib(lamaTilawah);
    eprom->setIndexAdzanMaghrib(adzan);
    break;
  case 4:
    eprom->setIndexTilawahIsya(surah);
    eprom->setTimeTilawahIsya(lamaTilawah);
    eprom->setIndexAdzanIsya(adzan);
    break;
  case 5:
    eprom->setIndexTilawahJumat(surah);
    eprom->setTimeTilawahJumat(lamaTilawah);
    eprom->setIndexAdzanJumat(adzan);
    break;
  }
  // 15 15 15 15 15 15 YNNNYY  //Subuh Dzuhur...  jumat YES/NO
  // PARAMETER.tartil_subuh = ((data[0] - '0') * 10) + (data[1] - '0');
  // PARAMETER.tartil_duhur = ((data[2] - '0') * 10) + (data[3] - '0');
  // PARAMETER.tartil_ashar = ((data[4] - '0') * 10) + (data[5] - '0');
  // PARAMETER.tartil_maghrib = ((data[6] - '0') * 10) + (data[7] - '0');
  // PARAMETER.tartil_isya = ((data[8] - '0') * 10) + (data[9] - '0');
  // PARAMETER.tartil_jumat = ((data[10] - '0') * 10) + (data[11] - '0');
  // if (data[12] == 'N')
  //   PARAMETER.tartil_subuh = 0;
  // if (data[13] == 'N')
  //   PARAMETER.tartil_duhur = 0; // ((_trtl[2]-'0')*10)+(_trtl[3]-'0');
  // if (data[14] == 'N')
  //   PARAMETER.tartil_ashar = 0; // ((_trtl[4]-'0')*10)+(_trtl[5]-'0') ;
  // if (data[15] == 'N')
  //   PARAMETER.tartil_maghrib = 0; //((_trtl[6]-'0')*10)+(_trtl[7]-'0');
  // if (data[16] == 'N')
  //   PARAMETER.tartil_isya = 0; //((_trtl[8]-'0')*10)+(_trtl[9]-'0');
  // if (data[17] == 'N')
  //   PARAMETER.tartil_jumat = 0; //((_trtl[10]-'0')*10)+(_trtl[11]-'0');
  // EEPROM.put(1, PARAMETER);
  Serial1.print("SetTrkm\n");
}
void Bluetooth::setOffsite(String &data)
{
  command = command_end;
  status = 1;
  // 1234 %F 19 02 01 03 05 //1= TAMBAH & 0=KURANG
  if (data[0] == '0')
  {
    eprom->setTambahKurangSubuh(0 - (data[1] - '0'));
  }
  else
  {
    eprom->setTambahKurangSubuh(data[1] - '0');
  }
  if (data[2] == '0')
  {
    eprom->setTambahKurangDuhur(0 - (data[3] - '0'));
  }
  else
  {
    eprom->setTambahKurangDuhur(data[3] - '0');
  }
  if (data[4] == '0')
  {
    eprom->setTambahKurangAshar(0 - (data[5] - '0'));
  }
  else
  {
    eprom->setTambahKurangAshar(data[5] - '0');
  }
  if (data[6] == '0')
  {
    eprom->setTambahKurangMaghrib(0 - (data[7] - '0'));
  }
  else
  {
    eprom->setTambahKurangMaghrib(data[7] - '0');
  }
  if (data[8] == '0')
  {
    eprom->setTambahKurangIsya(0 - (data[9] - '0'));
  }
  else
  {
    eprom->setTambahKurangIsya(data[9] - '0');
  }
  Serial1.print("SetOffs\n");
}
void Bluetooth::setText(String &data)
{
  status = 2;
  command = command_end;
  // 1234 %S tes karakter
  // writeString(text_run,input_serial);
  int _size = data.length();
  int i;
  for (i = 0; i < _size; i++)
  {

    EEPROM.write(TEXT_RUN + i, data[i]);
  }
  EEPROM.write(TEXT_RUN + _size, '\0'); // Add termination null character for String Data
  Serial1.print("SetText\n");
  // alamat_eprom = 0;
  // display_eprom(text_run);
}
void Bluetooth::setFix(String &data)
{
  command = command_end;
  status = 1;
  // YYNYN 2200 0000 0000 0000 0000
  // 5 9 13 17 21
  int jadwal_fix_subuh = 0;
  int jadwal_fix_duhur = 0;
  int jadwal_fix_ashar = 0;
  int jadwal_fix_maghrib = 0;
  int jadwal_fix_isya = 0;
  if (data[0] == 'Y')
    jadwal_fix_subuh = ((((data[5] - '0') * 10) + (data[6] - '0')) * 60) + (((data[7] - '0') * 10) + (data[8] - '0'));
  if (data[1] == 'Y')
    jadwal_fix_duhur = ((((data[9] - '0') * 10) + (data[10] - '0')) * 60) + (((data[11] - '0') * 10) + (data[12] - '0'));
  if (data[2] == 'Y')
    jadwal_fix_ashar = ((((data[13] - '0') * 10) + (data[14] - '0')) * 60) + (((data[15] - '0') * 10) + (data[16] - '0'));
  if (data[3] == 'Y')
    jadwal_fix_maghrib = ((((data[17] - '0') * 10) + (data[18] - '0')) * 60) + (((data[19] - '0') * 10) + (data[20] - '0'));
  if (data[4] == 'Y')
    jadwal_fix_isya = ((((data[21] - '0') * 10) + (data[22] - '0')) * 60) + (((data[23] - '0') * 10) + (data[24] - '0'));
  // EEPROM.put(1, PARAMETER);
  eprom->setJadwalFixSubuh(jadwal_fix_subuh);
  eprom->setJadwalFixDzuhur(jadwal_fix_duhur);
  eprom->setJadwalFixAshar(jadwal_fix_ashar);
  eprom->setJadwalFixMaghrib(jadwal_fix_maghrib);
  eprom->setJadwalFixIsya(jadwal_fix_isya);
  Serial1.print("SetFixx\n");
}
void Bluetooth::setBtKota(String &data)
{
  command = command_end;
  // EN 110 39 007 82 01187+00 //LU
  // ES 110 39 007 82 01187+00 //ls
  status = 1;
  float lnt;
  float bjr = ((data[2] - '0') * 100) + ((data[3] - '0') * 10) + (data[4] - '0') + ((data[5] - '0') * 0.1) + ((data[6] - '0') * 0.01);
  if (data[1] == 'N')
    lnt = ((data[7] - '0') * 100) + ((data[8] - '0') * 10) + (data[9] - '0') + ((data[10] - '0') * 0.1) + ((data[11] - '0') * 0.01);
  if (data[1] == 'S')
    lnt = 0 - (((data[7] - '0') * 100) + ((data[8] - '0') * 10) + (data[9] - '0') + ((data[10] - '0') * 0.1) + ((data[11] - '0') * 0.01));
  // unsigned char gmt = data[16] - '0';
  eprom->setKota(0);
  eprom->setKotaBujur(bjr);
  eprom->setKotaLintang(lnt);
  eprom->setKotaGMT(data[16] - '0');

  Serial1.print("SetKoor\n");
}
void Bluetooth::setBuzer(String &data)
{
  command = command_end;
  // 1234 %Y 12
  unsigned char bzr = ((data[0] - '0') * 10) + (data[1] - '0');
  eprom->setBeep(bzr);
  Serial1.print("SetBuzer\n");
}
void Bluetooth::cmdStart(String &data)
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
  if (data[0] == '%')
  {
    for (uint8_t cmd = 0; cmd < 13; cmd++)
    {
      char lookupCmd = bloothtCommand[cmd];
      if (lookupCmd == data[1])
      {
        Serial1.print("OK");
        Serial1.print(lookupCmd);
        Serial1.print("\n");
        command = cmd + 2;

        break;
      }
    }
  }
};
void Bluetooth::setBrightness(String &data)
{
  command = command_end;
  // 1234 %B 0300 01 0600 03 1730 01 2100 00
  // 1234 %B 0300 07 0600 03 1730 01 2100 00
  // PARAMETER.kecerahan_1 = input_serial[5] - '0';
  // PARAMETER.kecerahan_2 = input_serial[11] - '0';
  // PARAMETER.kecerahan_3 = input_serial[17] - '0';
  // PARAMETER.kecerahan_4 = input_serial[23] - '0';
  // PARAMETER.jam_kecerahan_1 = ((((input_serial[0] - '0') * 10) + (input_serial[1] - '0')) * 60) + (((input_serial[2] - '0') * 10) + (input_serial[3] - '0'));
  // PARAMETER.jam_kecerahan_2 = ((((input_serial[6] - '0') * 10) + (input_serial[7] - '0')) * 60) + (((input_serial[8] - '0') * 10) + (input_serial[9] - '0'));
  // PARAMETER.jam_kecerahan_3 = ((((input_serial[12] - '0') * 10) + (input_serial[13] - '0')) * 60) + (((input_serial[14] - '0') * 10) + (input_serial[15] - '0'));
  // PARAMETER.jam_kecerahan_4 = ((((input_serial[18] - '0') * 10) + (input_serial[19] - '0')) * 60) + (((input_serial[20] - '0') * 10) + (input_serial[21] - '0'));
  // EEPROM.put(1, PARAMETER);
  Serial1.print("SetBrns\n");
}
void Bluetooth::resetPabrik(String &data)
{
  command = command_end;
  // 1234 %R 123 ->lokasi
  int lokasi = ((data[0] - '0') * 100) + ((data[1] - '0') * 10) + (data[2] - '0');
  eprom->setKota(lokasi);
  status = 3;
  Serial1.print("Reset\n");
}
void Bluetooth::loop()
{
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
    switch (command)
    {
    case command_start:
      cmdStart(input_serial);
      break;
    case RESET_PABRIK:
      resetPabrik(input_serial);
      break;
    case SET_BUZER:
      setBuzer(input_serial);
      break;
    case SET_POWER:
      setPower(input_serial);
      break;
    case play_mp3:
      playManual(input_serial);
      break;
    case set_jam:
      setTime(input_serial);
      break;
    case set_text:
      setText(input_serial);
      break;
    case set_iqomah:
      setIqomah(input_serial);
      break;
    case set_tarhim:
      setTarhim(input_serial);
      break;
    case set_brightnes:
      setBrightness(input_serial);
      break;
    case set_offsite:
      setOffsite(input_serial);
      break;
    case set_fix:
      setFix(input_serial);
      break;
    case set_bt_kota:
      setBtKota(input_serial);
      break;
    case set_adzan:
      setAdzan(input_serial);
      break;
    default:
      break;
    }
  }
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
