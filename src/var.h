#if !defined(var_h)
#define var_h

#define buzerPin 2

#define tombol_up 5
#define tombol_menu 45
#define tombol_down 46

byte sqw;
// unsigned long previousMillis = 0;

//===============================================================================
// seting
// volatile static byte commad;

#define text_iq_subuh 100
#define text_iq_duhur 200
#define text_iq_ashar 300
#define text_iq_maghrib 400
#define text_iq_isya 500
#define text_iq_jumat 600
#define text_run 700

// #define command_start 1
// #define set_jam 2
// #define set_text 3
// #define set_iqomah 4
// #define set_tarhim 5
// #define set_brightnes 6
// #define set_offsite 7
// #define set_fix 8
// #define set_bt_kota 9
// #define set_adzan 10
// #define play_mp3 11
// #define command_end 12
// volatile static byte command;


#define waktu_imsya 7
#define waktu_subuh 8
#define waktu_duhur 9
#define waktu_ashar 10
#define waktu_magrib 11
#define waktu_isya 12
#define waktu_jumat 13
#define display_off 14
#define waktu_suruq 15

// #define jadwal 14
#define all 15

#define senin 1
#define selasa 2
#define rabu 3
#define kamis 4
#define jumat 5
#define sabtu 6
#define ahad 7
#define alarm_beep 8
#define on 9
#define off 10
#define imsya 11
#define subuh 12
#define duhur 13
#define ashar 14
#define magrib 15
#define isya 16
#define stanbay 17
#define iqomah 18
#define set_kota 19
#define tmbah_kurang 20
#define tlwh_1 21
#define tlwh_2 22
#define tlwh_3 23
#define tlwh_4 24
#define tlwh_5 25
#define tlwh_6 26
#define kutbah 27
#define segmenisrq 29

// data di kurang 1
// const PROGMEM uint8_t dataKalender[] = {
//     ks, ke, kn, ki, kn, k_, k_, k_,               // 0
//     ks, ke, kl, ka, ks, ka, k_, k_,               // 1
//     kr, ka, kb, ku, k_, k_, k_, k_,               // 2
//     kk, ka, km1, km2, ki, ks, k_, k_,             // 3
//     kj, ku, km1, km2, ka, kt, k_, k_,             // 4
//     ks, ka, kb, kt, ku, k_, k_, k_,               // 5
//     ka, kh, ka, kd, k_, k_, k_, k_,               // 6
//     ka, kl, ka, kr, km1, km2, k_, k_,             // 7
//     ko, kn, k_, k_, k_, k_, k_, k_,               // 8
//     ko, kf, kf, k_, k_, k_, k_, k_,               // 9
//     ki, km1, km2, ks, ky, ka, k_, k_,             // 10
//     ks, ku, kb, ku, kh, k_, k_, k_,               // 11
//     kd, ku, kh, ku, kr, k_, k_, k_,               // 12
//     ka, ks, kh, ka, kr, k_, k_, k_,               // 13
//     km1, km2, ka, kg, kh, kr, ki, kb,             // 14
//     ki, ks, ky, ka, k_, k_, k_, k_,               // 15
//     ks, kt, ka, kn, kb, ky, k_, k_,               // 16
//     ki, kq, ko, km1, km2, ka, kh, k_,             // 17
//     kk, ko, kt, ka, k_, k_, k_, k_,               // 18
//     kj, ka, kd, kw1, kw2, ka, kl, k_,             // 19
//     kt, ka, kr, kt, ki, kl, k_, data_kalender[1], // 20
//     kt, ka, kr, kt, ki, kl, k_, data_kalender[2], // 21
//     kt, ka, kr, kt, ki, kl, k_, data_kalender[3], // 22
//     kt, ka, kr, kt, ki, kl, k_, data_kalender[4], // 23
//     kt, ka, kr, kt, ki, kl, k_, data_kalender[5], // 24
//     kt, ka, kr, kt, ki, kl, k_, data_kalender[6], // 25
//     kk, kh, ku, kt, kb, ka, kh, k_,               // 26
//     k_, k_, k_, k_, k_, k_, k_, k_,               // 27
//     // kd,ku,kh,ka,k_,k_,k_,k_, //28
//     // ki,ks,kr,ka,kq,k_,k_,k_, //28
//     ks, ku, kr, ku, kq, k_, k_, k_, // 28

// };



#endif // var_h





  // String input_serial;
  // if (Serial1.available())
  // {
  //   input_serial = Serial1.readString();
  // }
  // if (input_serial == "RESET")
  // {
  //   reset();
  //   command = command_end;
  // }

  // if (input_serial == "1234")
  // {
  //   Serial1.print("OK\n");
  //   command = command_start;
  //   return;
  // }
  // else
  // {
  //   if (command == command_start)
  //   {
  //     if (input_serial[0] == '%')
  //     {
  //       for (uint8_t cmd = 0; cmd < 10; cmd++)
  //       {
  //         char lookupCmd = pgm_read_byte_near(bloothtCommand + cmd);
  //         if (lookupCmd == input_serial[1])
  //         {
  //           Serial1.print("OK");
  //           Serial1.print(lookupCmd);
  //           Serial1.print("\n");
  //           command = cmd + 2;

  //           break;
  //         }
  //       }
  //     }
  //   }
  //   //===========================================================
  //   else if (command == play_mp3)
  //   {
  //     command = command_end;
  //     int _track = ((input_serial[1] - '0') * 100) + ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
  //     // 1234 %W Q001
  //     // 1234 %W Q001
  //     // 1234 %W Q001
  //     // 1234 %W Q004
  //     // sholawat
  //     // 1234 %W W003
  //     // 1234 %W W010
  //     // stop
  //     // 1234 %W STOP
  //     // myDFPlayer.play(1);  //Play the first mp3
  //     // myDFPlayer.stop();

  //     if (input_serial[0] == 'Q')
  //     {
  //       myDFPlayer.play(_track); // play alquran
  //     }
  //     if (input_serial[0] == 'W')
  //     { // play sholawat
  //       myDFPlayer.play(_track + 114);
  //     }
  //     if (input_serial[0] == 'S')
  //     {
  //       myDFPlayer.stop(); // STOP
  //     }
  //     Serial1.print("SetPlay\n");
  //   }
  //   else if (command == set_jam)
  //   {
  //     command = command_end;
  //     // 290417040319 // DTK_MNT_JAM_TGL_BLN_TH
  //     unsigned char jam = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
  //     unsigned char menit = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
  //     unsigned char detik = ((input_serial[0] - '0') * 10) + (input_serial[1] - '0');
  //     unsigned char tanggal = ((input_serial[6] - '0') * 10) + (input_serial[7] - '0');
  //     unsigned char bulan = ((input_serial[8] - '0') * 10) + (input_serial[9] - '0');
  //     int tahun = 2000 + ((input_serial[10] - '0') * 10) + (input_serial[11] - '0');
  //     // tulis_rtc();
  //     myRtc.setJam(jam, menit, detik);
  //     myRtc.setTanggal(tanggal, bulan, tahun);
  //     Serial1.print("SetTime\n");
  //   }
  //   else if (command == set_text)
  //   { // ok

  //     command = command_end;
  //     // 1234 %S tes karakter
  //     // writeString(text_run,input_serial);
  //     int _size = input_serial.length();
  //     int i;
  //     for (i = 0; i < _size; i++)
  //       EEPROM.write(text_run + i, input_serial[i]);
  //     EEPROM.write(text_run + _size, '\0'); // Add termination null character for String Data
  //     Serial1.print("SetText\n");
  //     alamat_eprom = 0;
  //     display_eprom(text_run);
  //   }
  //   else if (command == set_iqomah)
  //   {
  //     // 1234 %I N 0 1010 Iqomah Subuh // n DAN y ADALAH ENABLE IQOMAH
  //     // 1234 %I N 1 1010 Iqomah Dzuhur... //IQOMAH_SHOLAT
  //     // 1234 %I N 2 1010 Iqomah Ashar...
  //     // 1234 %I N 3 1010 Iqomah Maghrib...
  //     // 1234 %I N 4 1010 Iqomah Isya...
  //     // 1234 %I N 5 0510 Iqomah jumat
  //     // 1234 %I Y 0 1010 Iqomah Subuh
  //     command = command_end;
  //     int _text_iqomah;
  //     if (input_serial[1] == '0')
  //     {
  //       parameter.iqomah_subuh = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
  //       parameter.lama_sholat_subuh = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
  //       // writeString(text_iq_subuh,input_serial.substring(6));
  //       _text_iqomah = text_iq_subuh;
  //     }
  //     if (input_serial[1] == '1')
  //     {
  //       parameter.iqomah_duhur = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
  //       parameter.lama_sholat_duhur = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
  //       // writeString(text_iq_duhur,input_serial.substring(6));
  //       _text_iqomah = text_iq_duhur;
  //     }
  //     if (input_serial[1] == '2')
  //     {
  //       parameter.iqomah_ashar = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
  //       parameter.lama_sholat_ashar = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
  //       // writeString(text_iq_ashar,input_serial.substring(6));
  //       _text_iqomah = text_iq_ashar;
  //     }
  //     if (input_serial[1] == '3')
  //     {
  //       parameter.iqomah_maghrib = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
  //       parameter.lama_sholat_maghrib = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
  //       // writeString(text_iq_maghrib,input_serial.substring(6));
  //       _text_iqomah = text_iq_maghrib;
  //     }
  //     if (input_serial[1] == '4')
  //     {
  //       parameter.iqomah_isya = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
  //       parameter.lama_sholat_isya = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
  //       // writeString(text_iq_isya,input_serial.substring(6));
  //       _text_iqomah = text_iq_isya;
  //     }
  //     if (input_serial[1] == '5')
  //     {
  //       parameter.iqomah_jumat = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
  //       parameter.lama_sholat_jumat = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
  //       // writeString(text_iq_jumat,input_serial.substring(6));
  //       _text_iqomah = text_iq_jumat;
  //     }
  //     int _size = input_serial.length();
  //     int i, u = 0;
  //     for (i = 6; i < _size; i++)
  //     {
  //       EEPROM.write(_text_iqomah + u, input_serial[i]);
  //       u++;
  //     }
  //     EEPROM.write(_text_iqomah + u, '\0');
  //     EEPROM.put(0, parameter);
  //     Serial1.print("SetIqom\n");
  //   }
  //   else if (command == set_tarhim)
  //   { // OK
  //     command = command_end;
  //     // 15 15 15 15 15 15 YNNNYY  //Subuh Dzuhur...  jumat YES/NO
  //     parameter.tartil_subuh = ((input_serial[0] - '0') * 10) + (input_serial[1] - '0');
  //     parameter.tartil_duhur = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
  //     parameter.tartil_ashar = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
  //     parameter.tartil_maghrib = ((input_serial[6] - '0') * 10) + (input_serial[7] - '0');
  //     parameter.tartil_isya = ((input_serial[8] - '0') * 10) + (input_serial[9] - '0');
  //     parameter.tartil_jumat = ((input_serial[10] - '0') * 10) + (input_serial[11] - '0');
  //     if (input_serial[12] == 'N')
  //       parameter.tartil_subuh = 0;
  //     if (input_serial[13] == 'N')
  //       parameter.tartil_duhur = 0; // ((_trtl[2]-'0')*10)+(_trtl[3]-'0');
  //     if (input_serial[14] == 'N')
  //       parameter.tartil_ashar = 0; // ((_trtl[4]-'0')*10)+(_trtl[5]-'0') ;
  //     if (input_serial[15] == 'N')
  //       parameter.tartil_maghrib = 0; //((_trtl[6]-'0')*10)+(_trtl[7]-'0');
  //     if (input_serial[16] == 'N')
  //       parameter.tartil_isya = 0; //((_trtl[8]-'0')*10)+(_trtl[9]-'0');
  //     if (input_serial[17] == 'N')
  //       parameter.tartil_jumat = 0; //((_trtl[10]-'0')*10)+(_trtl[11]-'0');
  //     EEPROM.put(0, parameter);
  //     Serial1.print("SetTrkm\n");
  //   }
  //   else if (command == set_brightnes)
  //   { // ok
  //     command = command_end;
  //     // 1234 %B 0300 01 0600 03 1730 01 2100 00
  //     // 1234 %B 0300 07 0600 03 1730 01 2100 00
  //     parameter.kecerahan_1 = input_serial[5] - '0';
  //     parameter.kecerahan_2 = input_serial[11] - '0';
  //     parameter.kecerahan_3 = input_serial[17] - '0';
  //     parameter.kecerahan_4 = input_serial[23] - '0';
  //     parameter.jam_kecerahan_1 = ((((input_serial[0] - '0') * 10) + (input_serial[1] - '0')) * 60) + (((input_serial[2] - '0') * 10) + (input_serial[3] - '0'));
  //     parameter.jam_kecerahan_2 = ((((input_serial[6] - '0') * 10) + (input_serial[7] - '0')) * 60) + (((input_serial[8] - '0') * 10) + (input_serial[9] - '0'));
  //     parameter.jam_kecerahan_3 = ((((input_serial[12] - '0') * 10) + (input_serial[13] - '0')) * 60) + (((input_serial[14] - '0') * 10) + (input_serial[15] - '0'));
  //     parameter.jam_kecerahan_4 = ((((input_serial[18] - '0') * 10) + (input_serial[19] - '0')) * 60) + (((input_serial[20] - '0') * 10) + (input_serial[21] - '0'));
  //     EEPROM.put(0, parameter);
  //     Serial1.print("SetBrns\n");
  //   }
  //   else if (command == set_offsite)
  //   { // OK
  //     command = command_end;
  //     // 1234 %F 19 02 01 03 05 //1= TAMBAH & 0=KURANG
  //     if (input_serial[0] == '0')
  //       parameter.tambah_kurang_subuh = 0 - (input_serial[1] - '0');
  //     else
  //       parameter.tambah_kurang_subuh = input_serial[1] - '0';
  //     if (input_serial[2] == '0')
  //       parameter.tambah_kurang_duhur = 0 - (input_serial[3] - '0');
  //     else
  //       parameter.tambah_kurang_duhur = input_serial[3] - '0';
  //     if (input_serial[4] == '0')
  //       parameter.tambah_kurang_ashar = 0 - (input_serial[5] - '0');
  //     else
  //       parameter.tambah_kurang_ashar = input_serial[5] - '0';
  //     if (input_serial[6] == '0')
  //       parameter.tambah_kurang_maghrib = 0 - (input_serial[7] - '0');
  //     else
  //       parameter.tambah_kurang_maghrib = input_serial[7] - '0';
  //     if (input_serial[8] == '0')
  //       parameter.tambah_kurang_isya = 0 - (input_serial[9] - '0');
  //     else
  //       parameter.tambah_kurang_isya = input_serial[9] - '0';
  //     EEPROM.put(0, parameter);
  //     Serial1.print("SetOffs\n");
  //   }
  //   else if (command == set_fix)
  //   { // OK
  //     command = command_end;
  //     // YYNYN 2200 0000 0000 0000 0000
  //     // 5 9 13 17 21
  //     parameter.jadwal_fix_subuh = 0;
  //     parameter.jadwal_fix_duhur = 0;
  //     parameter.jadwal_fix_ashar = 0;
  //     parameter.jadwal_fix_maghrib = 0;
  //     parameter.jadwal_fix_isya = 0;
  //     if (input_serial[0] == 'Y')
  //       parameter.jadwal_fix_subuh = ((((input_serial[5] - '0') * 10) + (input_serial[6] - '0')) * 60) + (((input_serial[7] - '0') * 10) + (input_serial[8] - '0'));
  //     if (input_serial[1] == 'Y')
  //       parameter.jadwal_fix_duhur = ((((input_serial[9] - '0') * 10) + (input_serial[10] - '0')) * 60) + (((input_serial[11] - '0') * 10) + (input_serial[12] - '0'));
  //     if (input_serial[2] == 'Y')
  //       parameter.jadwal_fix_ashar = ((((input_serial[13] - '0') * 10) + (input_serial[14] - '0')) * 60) + (((input_serial[15] - '0') * 10) + (input_serial[16] - '0'));
  //     if (input_serial[3] == 'Y')
  //       parameter.jadwal_fix_maghrib = ((((input_serial[17] - '0') * 10) + (input_serial[18] - '0')) * 60) + (((input_serial[19] - '0') * 10) + (input_serial[20] - '0'));
  //     if (input_serial[4] == 'Y')
  //       parameter.jadwal_fix_isya = ((((input_serial[21] - '0') * 10) + (input_serial[22] - '0')) * 60) + (((input_serial[23] - '0') * 10) + (input_serial[24] - '0'));
  //     EEPROM.put(0, parameter);
  //     Serial1.print("SetFixx\n");
  //   }
  //   else if (command == set_bt_kota)
  //   { // ok
  //     // EN 110 39 007 82 01187+00 //LU
  //     // ES 110 39 007 82 01187+00 //ls
  //     parameter.set_kota_bjr = ((input_serial[2] - '0') * 100) + ((input_serial[3] - '0') * 10) + (input_serial[4] - '0') + ((input_serial[5] - '0') * 0.1) + ((input_serial[6] - '0') * 0.01);
  //     if (input_serial[1] == 'N')
  //       parameter.set_kota_lnt = ((input_serial[7] - '0') * 100) + ((input_serial[8] - '0') * 10) + (input_serial[9] - '0') + ((input_serial[10] - '0') * 0.1) + ((input_serial[11] - '0') * 0.01);
  //     if (input_serial[1] == 'S')
  //       parameter.set_kota_lnt = 0 - (((input_serial[7] - '0') * 100) + ((input_serial[8] - '0') * 10) + (input_serial[9] - '0') + ((input_serial[10] - '0') * 0.1) + ((input_serial[11] - '0') * 0.01));
  //     parameter.set_kota_gmt = input_serial[16] - '0';
  //     parameter.kota = 0;
  //     EEPROM.put(0, parameter);
  //     Serial1.print("SetKoor\n");
  //     command = command_end;
  //   }
  //   else if (command == set_adzan)
  //   { // OK
  //     command = command_end;
  //     parameter.timer_adzan_subuh = ((input_serial[0] - '0') * 10) + (input_serial[1] - '0');
  //     parameter.timer_adzan_duhur = ((input_serial[2] - '0') * 10) + (input_serial[3] - '0');
  //     parameter.timer_adzan_ashar = ((input_serial[4] - '0') * 10) + (input_serial[5] - '0');
  //     parameter.timer_adzan_maghrib = ((input_serial[6] - '0') * 10) + (input_serial[7] - '0');
  //     parameter.timer_adzan_isya = ((input_serial[8] - '0') * 10) + (input_serial[9] - '0');
  //     parameter.timer_adzan_jumat = ((input_serial[10] - '0') * 10) + (input_serial[11] - '0');
  //     EEPROM.put(0, parameter);
  //     Serial1.print("SetAlrm\n");
  //   }
  // }

  // //===========================================================
  // if (command == command_end)
  // {
  //   myBuzer.setOn();
  //   // baca_jadwal(parameter.kota);
  //   myBuzer.setOff();
  //   command = 0;
  // }