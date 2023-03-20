#include <Wire.h>
#include <Adafruit_BMP280.h>

#include <VolAnalyzer.h>
#include <Clap.h>

//#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
//#include <WiFiScan.h>
//#include <WiFiServer.h>
//#include <WiFiType.h>
//#include <WiFiUdp.h>
///
#include <SPIFFS.h>
#include "FS.h"
///
#include <Arduino.h>
#include <Preferences.h>
//#include "driver/timer.h"
#include "esp32-hal-timer.h"
#include "driver/dac.h"
#include "XT_DAC_Audio.h"
#include "voice.h"
#include "SoundData.h"
#include "time.h"
#include <WebServer.h>
#include <Update.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <AsyncUDP.h>

#define DEBUG0
//#define DEBUG1
//#define DEBUG2
const String hstname_DEF = "CLK";
String hstname;
const String datname_DEF = "DS18b20";
String datname;
const String datchar_DEF = "C";
String datchar;
const String bmp0_datname_DEF = "BMP280_t";
String bmp0_datname;
const String bmp0_datchar_DEF = "C";
String bmp0_datchar;
const String bmp1_datname_DEF = "BMP280_p";
String bmp1_datname;
const String bmp1_datchar_DEF = "P";
String bmp1_datchar;
//
String datus[5];
const String lp1_DEF = "NON";
//String lp1;
const String lp2_DEF = "NON";
//String lp2;
const String mc1_DEF = "NON";
const String mc2_DEF = "NON";
const String mc3_DEF = "NON";
//String mc;
String t_radio[] {"lp1","lp2","mc1","mc2","mc3"};
//
  String content ="";
//
bool radf[5] {0,0,0,0,0};
//
const String ssidAP_DEF = "yourAPA";
const String passwdAP_DEF = "12345678";
String ssidAP;
String passwordAP;
const String local_ip_DEF = "192.168.111.2";
const String gateway_DEF = "192.168.111.2";
const String subnet_DEF = "255.255.255.0";
IPAddress local_ip;
IPAddress gateway;
IPAddress subnet;
String ssid;
String password;
//
bool cnt_wifi = 0;
Preferences preferences;  //Создаем экземпляр для создания хранилища
WebServer server(80);     //определяем порт WEB сервера
//
int port = 285;           //определяем порт для UDP броадкастов
AsyncUDP udp;
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
unsigned int web_ntp;
unsigned int cntwifi;  //переменная установки режима WiFi (1 - ST, 0 - AP)
const char *ntpServer = "pool.ntp.org";
String t_zone;
const String t_zone_DEF = "+3";  //Временная зона по умолчанию (UTC+3) Москва
long gmtOffset_sec;
int daylightOffset_sec;  //выключить переход на летнее время (для включения прописать 3600)
//WiFiUDP ntpUDP;                               //регистрируем NPT клиент для проверки синхронизации времени
//NTPClient timeClient(ntpUDP);                 //
volatile unsigned int interruptCounter = 0;
hw_timer_t *timer_0 = NULL;  //ссылка на указатель таймера
time_t now;
int hours;
volatile int h_hours;  //старшая цифра часов
volatile int l_hours;  //младшая цифра часов
int minutes;
volatile int h_minutes;  //старшая цифра минут
volatile int l_minutes;  //младшая цифра минут
int seconds;
volatile int h_seconds;
volatile int l_seconds;
String myhours;    //переменные для установки времени  без NTP
String myminutes;  //
bool synfl;        //флаг для синхронизации по NTP
int i59;
bool i30;
bool i35;
bool m0;
bool h12;
int tempfl;
float temperatureC;
int tint;
int l_tint;
int h_tint;
int s_tint;
//
String packetdata;
char *tmp_string;
String nam[4];
String t_name;
String t_data;
String t_char;
String brd[5][10];                       //массив броадкастов (имя, значение)
uint8_t brdn[] {3,3,3,3,3,3,3,3,3,3};     //массив устаревания броадкастов
float brdatf=0;
String brdatc;
int brdati=0;
uint8_t brdf=0;
uint8_t obsbf=0;
char broadcast_[10];
//
int boom_hours;  //счетчик боя курантов
String volume;   //громкость боя курантов
int boy;         //включение/выключение боя курантов
int voice;       //включение/выключение голоса
int cshelk1;     //включение/выклчение щелчка 1
int cshelk2;     //включение/выклчение щелчка 2
//
int fcount= 0;
int count = 0;
volatile uint8_t clapco=0; //счетчик времени вывода данных после хлопка
volatile int dynm = 0;  //анодный счетчик динамической индикации
//
//
//uint8_t anods[5] {0,4,13,14,15};         //массив выводов анодов
volatile uint8_t anods[7]{ 0, 32, 4, 12, 13, 15, 14 };  //массив выводов анодов
//volatile uint8_t segs[7]{ 16, 17, 18, 19, 21, 22, 5 };  //массив выводов сегментов 23 22
volatile uint8_t segs[7]{ 16, 17, 18, 19, 2, 33, 5 };  //массив выводов сегментов 21 -> 2, 2 -> 33
//
//volatile uint8_t sgm_m[13][7] = {
volatile bool sgm_m[15][7] = {
  { 1, 1, 1, 1, 1, 1, 0 },  //0
  { 0, 1, 1, 0, 0, 0, 0 },  //1
  { 1, 1, 0, 1, 1, 0, 1 },  //2
  { 1, 1, 1, 1, 0, 0, 1 },  //3
  { 0, 1, 1, 0, 0, 1, 1 },  //4
  { 1, 0, 1, 1, 0, 1, 1 },  //5
  { 1, 0, 1, 1, 1, 1, 1 },  //6
  { 1, 1, 1, 0, 0, 0, 0 },  //7
  { 1, 1, 1, 1, 1, 1, 1 },  //8
  { 1, 1, 1, 1, 0, 1, 1 },  //9
  { 0, 0, 0, 0, 0, 0, 0 },  //ZERRO 10
  { 1, 1, 0, 0, 0, 1, 1 },  //º     11
  { 0, 0, 0, 0, 0, 0, 1 },  //-     12
  { 1, 1, 0, 0, 1, 1, 1 },  //P     13
  { 1, 1, 0, 1, 0, 1, 1 }   //º_    14
};
//
//volatile uint8_t cur_dig[7][7] = {
volatile bool cur_dig[7][7] = {  
  { 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0 }
};
const int LED = 27;
const int BUTTON = 26;
// GPIO where the DS18B20 is connected to
const int oneWireBus = 23;
//
File root;
File  WavFile;
//
bool bmp_stat;
float gr;
float pr;
float alt;
//
//лапы ESP:
//25 - выход ЦАП
//A0 (GPIO 36) вход АЦП
//GPIO 12-15 32,4 - аноды индикатора (0,1,2,3,4,5) соответсвенно
//GPIO 16,17,18,19,21,22,5 - сегменты индикатора (A,B,C,D,E,F,G) соотвественно
//FIXMI 21 -> 2, 22 -> 33
//GPIO 26 кнопка
//GPIO 27 светодиоды
//GPIO 23 - ds18b20
///
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
VolAnalyzer vol(A0);	// микрофон на А0
Clap clap;
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);
DeviceAddress sensor0;
///
Adafruit_BMP280 bmp; // I2C
/////////////////////////////////////////////////////////////
//
void IRAM_ATTR Timer_0() {
      if (tempfl != 0)
      {
      digitalWrite(LED, LOW);
      }
    if (interruptCounter == 0) 
    {
      if(tempfl==2)
      {
        clapco++;
        if(clapco>=6)
        {
          tempfl=0;
          clapco=0;
        }
      }
    }
    if (interruptCounter == 500) {
      digitalWrite(LED, LOW);
    } 
  interruptCounter++;
  if(interruptCounter>=1000)
  {
    interruptCounter=0;
  }
//
      digitalWrite(anods[dynm], LOW);  //гасим текущий разряд
  dynm++;
    if (dynm == 7) {
    dynm = 1;
  }
//16, 17, 18, 19, 21, 22, 5  
//  digitalWrite(segs[7], cur_dig[dynm][7]);
  for(int i=0;i<7;i++)
  {
    digitalWrite(segs[i], cur_dig[dynm][i]);
  }
  digitalWrite(anods[dynm], HIGH);  //зажигаем текущий разряд
//
}
/////////////////////////////////////////////////////////////
String Ip2String(const IPAddress &ipAddress) {
  return String(ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") + String(ipAddress[2]) + String(".") + String(ipAddress[3]);
}
/////////////////////////////////////////////////////////////
//процедура опроса ds1820
//возвращает целую часть измеренной температуры или 255, если нет датчика
int ds1820(void) {
  sensors.getAddress(sensor0, 0);
//  Serial.println(sensors.getAddress(sensor0, 0));
  if (sensors.getAddress(sensor0, 0) >= 1) {
    sensors.requestTemperatures();
    temperatureC = sensors.getTempCByIndex(0);
    float temperatureF = sensors.getTempFByIndex(0);
//    Serial.print(temperatureC);
//    Serial.println("ºC");
    return (abs(temperatureC));
  } else {
    return (65535);
  }
}
/////////////////////////////////////////////////////////////
//процедура считывания данных с ds1820
void rd_ds1820 (void)
{
    tint = ds1820();  //производим измерение температуры
    if (tint == 65535) {
      Serial.println("Нет датчика");
    } else {
      broadcast(hstname, datname, temperatureC, datchar);
    }
}
/////////////////////////////////////////////////////////////
//процедура считывания данных с BMP280
void rd_bmp280 (void)
{
  if(bmp_stat)    //проверка наличия датчика bmp280
  {
  gr=bmp.readTemperature();
  pr=bmp.readPressure();
  pr=pr*0.00750062; // Преобразуем Паскали в мм.рт.ст.
  alt=bmp.readAltitude(1013.25);
#ifdef DEBUG0
  Serial.print("\nTemperature: ");
  Serial.print(gr-2.18);
  Serial.print(" C, ");
  Serial.print("\nPressure: ");
  Serial.print(pr);
  Serial.println(" Pa");
  Serial.print(pr);
  Serial.println(" мм.рт.ст.");
#endif
   broadcast(hstname, bmp1_datname, pr, bmp1_datchar);
   broadcast(hstname, bmp0_datname, gr-2.18, bmp0_datchar);
  }
}
/////////////////////////////////////////////////////////////
//Обработка события от WiFi при разрыве связи
void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Disconnected from WiFi access point");
  Serial.println("Trying to Reconnect");
  WiFi.begin(ssid.c_str(), password.c_str());  //соединяемся с WiFi
}
/////////////////////////////////////////////////////////////
//процедура установки режима WiFi
void cntrlWiFi(void) {
  preferences.begin("nvs", false);  //открываем хранилище nvs на чтене и запись
  if (cntwifi == 0) {
    //режим WiFi STA
    ssid = preferences.getString("ssid", "");
    if (ssid != "")  //если указан SSID
    {
      WiFi.disconnect();
      WiFi.mode(WIFI_STA);
      WiFi.disconnect();
      WiFi.setAutoReconnect(true);
      password = preferences.getString("password", "");
      WiFi.setHostname(hstname.c_str());           //учтанавливаем имя хоста
      WiFi.begin(ssid.c_str(), password.c_str());  //соединяемся с WiFi
      Serial.println("STA");
    } else  //если не указан SSID - переходим в режим AP
    {
      preferences.putUInt("cntwifi", 1);
      preferences.end();  //закрываем хранилище
      Serial.println("RESET 2 AP");
      ESP.restart();
    }
  } else {
    //режим WiFi AP
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    WiFi.mode(WIFI_AP);
    WiFi.disconnect();
    delay(100);
    ssidAP = preferences.getString("ssidAP", ssidAP_DEF);
    passwordAP = preferences.getString("passwordAP", passwdAP_DEF);
    WiFi.softAP(ssidAP.c_str(), passwordAP.c_str());
    delay(100);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    web_ntp = 0;  //выклбчаем NTP
    Serial.println("AP");
  }
  preferences.end();  //закрываем хранилище
}
/////////////////////////////////////////////////////////////
//
//
XT_Sequence_Class Sequence;               // The sequence object, you add your sounds above to this object (see setup below)
// create an object of type XT_Wav_Class that is used bythe dac audio class (below), passing wav data as parameter.
XT_Wav_Class ForceWithYou(Force);   //бой курантов
//XT_Wav_Class  Midnight(_00);        //полночь
//XT_Wav_Class  Noon(_12);            //полдень
XT_Wav_Class  Voice(_test);            //час
XT_DAC_Audio_Class DacAudio(25, 3);  // Create the main player class object.
// Use GPIO 25, one of the 2 DAC pins and timer 0
void play_wav (char* WavName)
{
          fcount=0;
          for(int i=0;i<=sizeof(_test);i++)
          {
            _test[i]=0;
          }
          WavFile = SPIFFS.open(WavName,FILE_READ);
          while(WavFile.available()) 
          {
              _test[fcount]=WavFile.read();
              fcount++;
          }
          WavFile.close();
          Sequence.AddPlayItem(&Voice);
}
/////////////////////////////////////////////////////////
//подготовка к выводу данных из броадкаста
/////////////////////////////////////////////////////////
void brdd(String datusstr, int cshelk, char* playsch, int volumsch, int tmpfl)
//datus[0], cshelk, "/cshelk1.wav", 10
//имя датчика в массиве, переменная-флаг включения озвучки,имя файла озучки (щелчка), громкосто озвучки, признак цикл/микрофон (0-часы, 1-цикл,2-хлопок)
 {
  for(int i=0;i<10;i++)
  {
    if((brd[0][i]==datusstr)&&(brdn[i]<3))
    {
      brdatf=brd[1][i].toFloat();
      brdatc=brd[2][i];
      Serial.print("brdatf=");
      Serial.print(brdatf);
      Serial.println(brdatc);
       tempfl = tmpfl;
       if(cshelk==1)
       {
       Sequence.RemoveAllPlayItems();            // Clear out any previous playlist
       play_wav(playsch);
       DacAudio.DacVolume = volumsch;  //Уровень громкости(0-100)
       DacAudio.Play(&Sequence);
       }
    }
  } 
 }
/////////////////////////////////////////////////////////
void setup() {
#ifdef DEBUG0
  Serial.begin(115200);
#endif
///////////////////BMP280/////////////////////////////////
  Serial.println(F("BMP280 test"));
  bmp_stat = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  if (!bmp_stat) 
  {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
  }
  else
  {
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X16,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  }
///////////////////END BMP280/////////////////////////////////
  //
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  pinMode(BUTTON, INPUT_PULLUP);  //опрос кнопки сброса в default при старте
  delay(100);                     //время для установления уровней после подачи питания
  if (!digitalRead(BUTTON)) {
    preferences.begin("nvs", false);  //открываем хранилище nvs на чтене и запись
    preferences.clear();              //сброс всех настроек
    preferences.end();                //закрываем хранилище
    Serial.println("Сброс всех настроек в default");
  }
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //////////////////////////////////////////////////////////////////////////////////////////
  //считываем настройки из EEPROM
  //////////////////////////////////////////////////////////////////////////////////////////
  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  preferences.begin("nvs", false);                          //открываем хранилище nvs на чтене и запись
  hstname = preferences.getString("hstname", hstname_DEF);  //считываем имя хоста. По умолчанию == CLK
  
  datname = preferences.getString("datname", datname_DEF);  //считываем имя датчика. По умолчанию == DS18b20
  datchar = preferences.getString("datchar", datchar_DEF);  //считываем единицы измерения датчика. По умолчанию == C
  
  bmp0_datname = preferences.getString("bmp0_datname", bmp0_datname_DEF);  //считываем имя датчика. По умолчанию == BMP280_t
  bmp0_datchar = preferences.getString("bmp0_datchar", bmp0_datchar_DEF);  //считываем единицы измерения датчика. По умолчанию == C

  bmp1_datname = preferences.getString("bmp1_datname", bmp1_datname_DEF);  //считываем имя датчика. По умолчанию == BMP280_p
  bmp1_datchar = preferences.getString("bmp1_datchar", bmp1_datchar_DEF);  //считываем единицы измерения датчика. По умолчанию == H
  
  datus[0]= preferences.getString("lp1", lp1_DEF);          //считываем какой датчик отображать в 1-м цикле (30-35 сек). По умолчанию == NON
  datus[1]= preferences.getString("lp2", lp2_DEF);          //считываем какой датчик отображать в 1-м цикле (35-40 сек). По умолчанию == NON
  datus[2]= preferences.getString("mc1", mc1_DEF);          //считываем какой датчик отображать по 1-му хлопку. По умолчанию == NON
  datus[3]= preferences.getString("mc2", mc2_DEF);          //считываем какой датчик отображать по 2-м хлопкам. По умолчанию == NON
  datus[4]= preferences.getString("mc3", mc3_DEF);          //считываем какой датчик отображать по 3-м хлопкам. По умолчанию == NON
  boy=preferences.getInt("boy", 1);                         //считываем включени или выключен бой. По умолчанию == 1 - включен
  voice=preferences.getInt("voice", 1);                     //считываем включени или выключен голос. По умолчанию == 1 - включен
  cshelk1=preferences.getInt("cshelk1", 1);                 //считываем включени или выключен щелчок 1. По умолчанию == 1 - включен
  cshelk2=preferences.getInt("cshelk2", 1);                 //считываем включени или выключен щелчок 2. По умолчанию == 1 - включен
  volume = preferences.getString("volume", "5");            //считываем значение громкости. По умолчанию == 5%
  web_ntp = preferences.getUInt("web_ntp", 0);              //считываем флаг синхронизации по NTP. Если флаг не существует - то возвращаем 0
  cntwifi = preferences.getUInt("cntwifi", 1);              //считываем флаг режима WiFi, если не существует - то STA

  ssid = preferences.getString("ssid", "");          //считываем значение SSID
  password = preferences.getString("password", "");  //считываем значение password

  String _local_ip = preferences.getString("local_ip", local_ip_DEF);  //считываем локальный IP
  local_ip.fromString(_local_ip);
  String _gateway = preferences.getString("gateway", gateway_DEF);  //считываем шлюз
  gateway.fromString(_gateway);
  String _subnet = preferences.getString("subnet", subnet_DEF);  //считываем маску
  subnet.fromString(_subnet);
  t_zone = preferences.getString("t_zone", t_zone_DEF);  //считываем временную зону
  gmtOffset_sec = t_zone.toInt();
  gmtOffset_sec = gmtOffset_sec * 3600;
  daylightOffset_sec = preferences.getInt("dst", 0);  //считываем переход на летнее время (по умолчанию - нет перехода"
  preferences.end();                                  //закрываем хранилище
  //      preferences.putUInt("volume", volume);
  // Удаляем отдельный ключ
  //preferences.remove("counter");
  //////////////////////////////////////////////////////////////////////////////////////////
  //
  // Инициализируем SPIFFS:
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  //
     root = SPIFFS.open("/");
 Serial.println("OK");
  File file = root.openNextFile();
 
  while(file){
      Serial.print("FILE: ");
      Serial.println(file.name());
      file = root.openNextFile();
  }
  root.close();
  ///  
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
  cntrlWiFi();   //устанавливаем режим WiFi
  cnt_wifi = 0;  //сбасываем флаг изменения настроек WiFi
  //
  //
  //////////////////////////////////////////////////////////////////////////////////////////
  //регистрируем корневые страницы WEB сервера
  server.on("/", handleRoot);
  server.on("/favicon.ico",handleFavicon);
  server.on("/jquery.js",handleJquery);
  server.on("/login", handleLogin);
  server.on("/wifi", handleWiFi);
  server.on("/datchik", handleDatchik);
  server.on("/admin", padmin);
//Web OTA
  server.on("/upgrade", HTTP_GET, form_ota);
//
  server.on("/upload",   File_Upload);
  server.on("/fupload",  HTTP_POST,[](){ server.send(200);}, handleFileUpload);
  server.on("/fremove", File_Remove);
//
  server.onNotFound(handleNotFound);
    update_ota();
  //here the list of headers to be recorded
  const char *headerkeys[] = { "User-Agent", "Cookie" };
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char *);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize);
  server.begin();
  Serial.println("HTTP server started");  //запускаем WEB сервер
  //////////////////////////////////////////////////////////////////////////////////////////
  //
  //
  //////////////////////////////////////////////////////////////////////////////////////////
  //иницируем порты с 12 по 19 и с 21 по 23 в режим GPIO и устанавливаем их на ВЫХОД

  gpio_config_t gpioConfig;  //| GPIO_SEL_23
  gpioConfig.pin_bit_mask = GPIO_SEL_4 | GPIO_SEL_5 | GPIO_SEL_12 | GPIO_SEL_13 | GPIO_SEL_14 | GPIO_SEL_15 | GPIO_SEL_16 | GPIO_SEL_17 | GPIO_SEL_18 | GPIO_SEL_19 | GPIO_SEL_21 | GPIO_SEL_22 | GPIO_SEL_27 | GPIO_SEL_32 | GPIO_SEL_33 | GPIO_SEL_2;
  gpioConfig.mode = GPIO_MODE_OUTPUT;
  gpioConfig.pull_up_en = GPIO_PULLUP_DISABLE;
  //gpioConfig.pull_up_en = GPIO_PULLUP_ENABLE;
  gpioConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
  //gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
  gpioConfig.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&gpioConfig);
//////////////////////////////////////////////////////////////////////////////////////////
  dac_output_disable(DAC_CHANNEL_1);
  pinMode(25, OUTPUT);
  //          pinMode(25, INPUT);
  //        dacWrite(DAC1, 0);
  //        digitalWrite(25, LOW);
  //////////////////////////////////////////////////////////////////////////////////////////
  timer_0 = timerBegin(0, 80, true);
  timerAttachInterrupt(timer_0, &Timer_0, true);
//  timerAlarmWrite(timer_0, 1000000, true);
  timerAlarmWrite(timer_0, 2000, true);  
  timerAlarmEnable(timer_0);
  //////////////////////////////////////////////////////////////////////////////////////////
  i59 = 0;
  i30 = 0;
  i35 = 0;
  m0  = 0;
  h12 = 0;
  boom_hours = 1;
  synfl = 1;

  //
  if (web_ntp != 0) {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);  // Синхронизируем внутренние часы со временем из интернета
  }

  for (int i = 0; i < 5; i++) {
    digitalWrite(anods[i], LOW);  //гасим все разряды
  }
  //инициализация ds18b20
    sensors.begin();
  tempfl = 0;
  serv_broad();     //прием броадкастов
  obsbf  = 0;
}
//
//////////////////////////////////////////////////////////////////////////////////////////
//
void loop() {

  //разбор времени в формат bcd
  time(&now);
  struct tm *local = localtime(&now);
  hours = local->tm_hour;
  h_hours = hours / 10;
  l_hours = hours - h_hours * 10;
  minutes = local->tm_min;
  h_minutes = minutes / 10;
  l_minutes = minutes - h_minutes * 10;
  seconds = local->tm_sec;
  h_seconds = seconds / 10;
  l_seconds = seconds - h_seconds * 10;
  //
  ///
////////////////////////////  
  //вывод времени
//  if ((tempfl == 0)&&(isf==1)) {
  if (tempfl == 0) {
    for (int i = 0; i < 7; i++) {
      cur_dig[6][i] = sgm_m[h_hours][i];
      cur_dig[5][i] = sgm_m[l_hours][i];
      cur_dig[4][i] = sgm_m[h_minutes][i];
      cur_dig[3][i] = sgm_m[l_minutes][i];
      cur_dig[2][i] = sgm_m[h_seconds][i];
      cur_dig[1][i] = sgm_m[l_seconds][i];
      digitalWrite(LED,(l_seconds&1));
    }
//    isf=0;
  }

////////////////////////////////////////////  
  //вывод показаний из бродкастов (температуры)
  if (tempfl != 0) 
  {
//    brdatc
      brdati=abs(brdatf);
      s_tint = brdati / 100;
      h_tint = (brdati - (s_tint*100))/ 10;
      l_tint = brdati - ((s_tint*100)+(h_tint * 10));
    for (int i = 0; i < 7; i++) 
    {

      if((brdatc=="C")||(brdatc=="c"))
      {
        if (brdatf < 0) 
        {
          cur_dig[6][i] = sgm_m[12][i];
        } 
        else 
        {

            cur_dig[6][i] = sgm_m[10][i];
        }
        if(brdatc=="C")
        {
          cur_dig[3][i] = sgm_m[11][i];       //вывод символа C
        }
        else
        {
          cur_dig[3][i] = sgm_m[14][i];       //вывод символа c
        }
      }
      if((brdatc=="P")||(brdatc=="p"))
      {
        cur_dig[6][i] = sgm_m[s_tint][i];
        cur_dig[3][i] = sgm_m[13][i];       //вывод символа P
      }
      cur_dig[5][i] = sgm_m[h_tint][i];
      cur_dig[4][i] = sgm_m[l_tint][i];
      cur_dig[2][i] = sgm_m[h_seconds][i];
      cur_dig[1][i] = sgm_m[l_seconds][i];
    }
  }

/////////////
  //установка времении при отключенном NTP
  /////////////
  if ((myminutes != "") && (myhours != "") && (web_ntp == 0)) {
    Serial.println("Set time");
    Serial.println(myhours);
    local->tm_hour = myhours.toInt();
    local->tm_min = myminutes.toInt();
    now = mktime(local);
    timeval tv = { now, 0 };
    timezone tz = { 0, 0 };
    settimeofday(&tv, &tz);
    myminutes = "";
    myhours = "";
  }
  /////////
  //синхронизация с NTP сервером (каждый час)

  if (synfl == 0) {
    //включение/выключение синхронизации по NTP
    if (web_ntp != 0) {
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);  // Синхронизируем внутренние часы со временем из интернета
    }
    synfl = 1;
  }
  //
  //кажую 30 секунду минуты
  //lp1
  if ((seconds == 30)&&(!i30)) 
  {
      rd_ds1820();        //производим измерение температуры
      rd_bmp280();        //производим измерение давления и температуры
      i30=1;
///////
      brdd(datus[0], cshelk1,"/cshelk1.wav",100,1);  //подготовка к выводу данных из броадкаста
///////
    if(obsbf==0)
    {
      obs_broadcast();    //проверяем на устаревание броадкастов
      obsbf=1;
    }    
  }
//кажую 35 секунду минуты
//lp2
  if ((seconds == 35)&&(!i35))
    {
      i35=1;
      i30=0;
      if(tempfl!=2)
      {
      tempfl = 0;
      }
//////////
      brdd(datus[1], cshelk2, "/cshelk2.wav",100,1);   //подготовка к выводу данных из броадкаста
//////////
    }
  
  //каждая 40-я секунда минуты
  if (seconds == 40)
  {
    i35=0;
//до какого времени производить вывод показаний датчика
    if(tempfl!=2)
    {
    tempfl = 0;
    }
    obsbf=0;          //разрешение на следующую проверку устаревания броадкастов
  }
  //
  //////////////////////////////////////////////////////////////////////////////////////////
  if ((minutes == 59)&&(!i59)) {
      synfl = 0;
      i59 = 1;
      m0 = 0;
      h12=0;
  }
  if (minutes == 0) {
    i59 = 0;
//    
    if(!m0)
    {
      Sequence.RemoveAllPlayItems();            // Clear out any previous playlist
////////////////////////////
//вывод голоса
char fnm[6]="";
    if(voice==1)
    {
    sprintf(fnm,"/%d.wav",hours);
    play_wav(fnm);
    }
////////////////////////////      
    //
      if (hours <= 12) {
        boom_hours = hours;
      } else {
        boom_hours = hours - 12;
      }
      if(hours==0)
      {
        boom_hours = 12;
      }
      if(boy==1)
      {
        while(boom_hours>0)
        {
          Sequence.AddPlayItem(&ForceWithYou);    //бой курантов
          boom_hours--;
        }
      }
       DacAudio.Play(&Sequence);
      m0  = 1;      
    }
//    
      dac_output_enable(DAC_CHANNEL_1);
      DacAudio.DacVolume = volume.toInt();  //Уровень громкости боя (0-100)
//!!!      DacAudio.FillBuffer();                // Fill the sound buffer with data
  }
    if((hours==0)&&(!h12))
    {
//      Sequence.RemoveAllPlayItems();            // Clear out any previous playlist
//      Sequence.AddPlayItem(&Midnight);
//          DacAudio.Play(&Midnight);
          h12=1;
    }
//    if (boom_hours >= 0) {
      //      SetVolume(64);
//      dac_output_enable(DAC_CHANNEL_1);
//      DacAudio.DacVolume = volume.toInt();  //Уровень громкости боя (0-100)
//      DacAudio.FillBuffer();                // Fill the sound buffer with data
/*
      if (ForceWithYou.Playing == false)    // if not playing,
      {
        boom_hours--;
        DacAudio.Play(&ForceWithYou);  // play it, this will cause it to repeat and repeat...
      }
    } else {
      dac_output_disable(DAC_CHANNEL_1);
      pinMode(25, OUTPUT);
      //        pinMode(25, INPUT);
      //          dacWrite(DAC1, 0);
      digitalWrite(25, LOW);
    }
*/
//  }
//  DacAudio.Play(&Sequence);
      DacAudio.FillBuffer();                // PLAY !!! Fill the sound buffer with data
  //прослушиваем HTTP запросы от клиента WEB
  server.handleClient();
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//определение громких звуков (хлопков   )
  vol.tick();
  clap.tick(vol.getRawMax());
  
//  if(clap.isClap())
  if (clap.hasClaps(1))
  {
    brdd(datus[2], 1, "/ding.wav",100,2);   //подготовка к выводу данных из броадкаста    
  }
    if (clap.hasClaps(2))
  {
    brdd(datus[3], 1, "/ding.wav",100,2);   //подготовка к выводу данных из броадкаста    
  }
    if (clap.hasClaps(3))
  {
    brdd(datus[4], 1, "/ding.wav",100,2);   //подготовка к выводу данных из броадкаста    
  }
//  if (clap.isClap()) Serial.println("clap");
//  if (clap.hasClaps(1)) Serial.println("1 clap");
//  if (clap.hasClaps(2)) Serial.println("2 clap");
//  if (clap.hasClaps(3)) Serial.println("3 clap");
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
//////////////////////////////////////////////////////////////////////////////////////////
