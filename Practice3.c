#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#define DHTTYPE DHT11

// LCD에 표시될 아이콘 매핑 배열
byte sms[8] = {
  B00000,
  B10001,
  B11011, 
  B01110,
  B10101,
  B11011,
  B11111,
  B00000,
};

byte alm[8] = {
  B00000,
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B11111,
  B00100
};

byte hmd[8] = {
  B00000,
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B01110
};

byte conn[8] = {
 B00000,
 B01110,
 B10001,
 B00100,
 B01010,
 B00000,
 B00100,
 B00000
};

byte unconn[8] = {
 B00000,
 B01111,
 B10011,
 B00100,
 B01010,
 B10000,
 B00100,
 B00000
};

byte cel[8] = {
 B11000,
 B11000,
 B01110,
 B10000,
 B10000,
 B10001,
 B01110,
 B00000
  
};

char buf[128];
int bufPos = 0;

// 핀 설정
const int DHT_PIN = 9;
const int PIEZO_PIN = 3;
const int LED_PIN_1 = 6;
const int LED_PIN_2 = 7;
const int LED_PIN_3 = 8;

// LCD 변수 - LCD 디스플레이를 제어한다 
LiquidCrystal_I2C lcd(0x3f, 16, 2);
SoftwareSerial ble(11, 12);

// DHT 변수 - 온습도 센서를 제어한다
DHT dht(DHT_PIN, DHTTYPE);

int hour = 60;
int minute = 0;
int blink_count = -1;
int first = -1;

bool blinkSMS = false;
bool blinkDoneSMS = false;
bool blinkALM = false;
bool blinkDoneALM = true;

String _str = "";
String _alm = "";

unsigned long beforeSMS;
unsigned long beforeALM;

void setup() {
  Wire.begin();
  // 블루투스
  ble.begin(9600);
  Serial.begin(9600);
  // 온습도 센서
  dht.begin();
  // 피에조 부저
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(13, OUTPUT);
  // LED
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  lcd.createChar(0, sms);
  lcd.createChar(1, alm);
  lcd.createChar(2, hmd);
  lcd.createChar(3, conn);
  lcd.createChar(4, unconn);
  lcd.createChar(5, cel);
  lcd.init(); // lcd 초기화
  lcd.backlight(); // lcd의 백라이트 활성화
  lcd.write(byte(4));
  lcd.print("Unconnected!");// 연결전 화면
} 

// 1번째 줄에 정보 현재 시간 및 상태 정보를 화면에 나타낸다 (현재 시간, 알림 텍스트, 온도)
void updateTime(bool conn, String _time, int temperature, bool interrupt){
 lcd.setCursor(0, 0);
 if (conn){
  if (!interrupt)
    lcd.print(_time);
  else{
    if (!blinkDoneSMS)
      lcd.print("SMS Recieved");
    else if (!blinkDoneALM)
      lcd.print("ALARM Rining");
  }
 }else{
  lcd.print("Unconnected!");
 }
 lcd.setCursor(13, 0);
 lcd.print((String)temperature);
 lcd.write(byte(5));
 
}

// 2번째 줄에 각종 상태 정보 값을 화면에 나타낸다 (SMS 알림, 알람 정보, 습도, 스마트폰과 연결상태)
void updateStatus(bool sms, bool alm, int humidity, bool conn){
   lcd.setCursor(0, 1);

   // sms가 온 경우
   lcd.write(byte(0));
   if (sms){
    if (!blinkDoneSMS)
      updateSMS();
   }else{
    lcd.print("--- ");
   }
   
   lcd.write(byte(1));
   if (alm){
    lcd.print("ALM ");
    //updateALM();
    checkALM();
   }else{
    lcd.print("--- ");
   }
   
   lcd.write(byte(2));
   String alarm = String(humidity) + "% ";
   lcd.print(alarm);
   
   if (conn) {
       lcd.write(byte(3));
   }else {
       lcd.write(byte(4));
   }
}

// SMS 정보를 수신 했을 때 화면에 띄워준다
void updateSMS(){
  if (blink_count == -1){
    blink_count= 0;
    beforeSMS = millis();
  }

    if (blinkSMS){
      lcd.print("SMS ");
      tone(PIEZO_PIN, 262, 500);
      if ((millis() - beforeSMS) >= (unsigned long) 1000){
        blinkSMS = false;
        ++blink_count;
        beforeSMS = millis();
      }
    }else {
      lcd.setCursor(1,1);
      lcd.print("    ");
      if ((millis() - beforeSMS) >= (unsigned long) 1000){
        blinkSMS = true;      
        ++blink_count;
        beforeSMS = millis();

      }
    }
  if (blink_count == 10){
    blink_count = -1;
    blinkDoneSMS = true;
  }
}

// 현재 시간과 알람에 설정된 시간과 비교하여 같은 같아지면 알람을 울리고 이를 폰으로 보낸다
void checkALM(){
  if (_str.length() > 13)
    if (_alm.equals(_str.substring(2, 10)) && !blinkALM){
      blinkALM = true;
      blinkDoneALM = false;
      tone(PIEZO_PIN, 264);
      ble.write(1);
    }
  
}

void updateALM(){
  if (first == -1){
    first =  0;
    beforeALM = millis();
  }

  if ( (millis() - beforeALM) >= 5000) {
    blinkDoneALM = true;
  }
}

void blinkLED1() {
  digitalWrite(LED_PIN_1, HIGH);
}

void blinkLED2() {
  digitalWrite(LED_PIN_1, HIGH);  
}

void blinkLED3() {
  digitalWrite(LED_PIN_1, HIGH);
}

bool isinterrupt(){
  if (!blinkDoneSMS || blinkALM ){
    return true;
  }
  return false;
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  while(ble.available()){
    digitalWrite(13, HIGH);
    char data = ble.read();
    
    if(data == '\n'){
        // 문자열 끝 
       Serial.println(_str);

      if (_str[1] == '!'){
        _str.remove(0, 2); // from index 0 remove count of 2 (\n, !)
        updateTime(true, _str, temperature, isinterrupt());                
      }else if(_str[1] == '2'){
        // sms 수신
        blinkDoneSMS = false;
        updateTime(true, _str, temperature, isinterrupt());
      }else if (_str[1] == '3'){
        // 알람 정보 수신 
        _str.remove(0, 2);
        _alm = _str;
        Serial.println(_alm);
        blinkDoneALM = false;
        blinkLED1();
        updateTime(true, _str, temperature, isinterrupt());
      }else if (_str[1] == '4'){
        // 알람 끄기
        blinkDoneALM = true;
        blinkALM = false;
        noTone(PIEZO_PIN);
      }
      ble.write(humidity);
      _str = "";
    }
    _str += data;
    updateStatus(!blinkDoneSMS, !blinkDoneALM, humidity, true);
    
    // 온습도 데이터 전송
  }
  digitalWrite(13, LOW);
   
}