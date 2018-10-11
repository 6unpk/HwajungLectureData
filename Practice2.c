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

// 핀 설정
const int DHT_PIN = 9;
const int PIEZO_PIN = 3;
const int LED_PIN_1 = 6;
const int LED_PIN_2 = 7;
const int LED_PIN_3 = 8;

// LED BLINK INTERVAL - LED 가 몇초 마다 깜빡이게 할지 정한다

const int INTERVAL_LED1 = 500; // 0.5초마다 
const int INTERVAL_LED2 = 1000; // 1초 마다
const int INTERVAL_LED3 = 1500; // 1.5초 마다 

unsigned long led1, led2, led3;
bool on1, on2, on3;
// LCD 변수 - LCD 디스플레이를 제어한다 
LiquidCrystal_I2C lcd(0x3f, 16, 2);
SoftwareSerial ble(11, 12);

// DHT 변수 - 온습도 센서를 제어한다
DHT dht(DHT_PIN, DHTTYPE);

void setup() {
  Wire.begin();
  // 블루투스
  ble.begin(9600);
  Serial.begin(9600);
  Serial.println("Input Your String (English or Number Only): ");
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
  lcd.setCursor(1, 0); // 1행 0열 부터 출력 
  lcd.print("Hello World!"); // 출력할 문구 (ASCII만 가능함)
} 

void blinkLED1(){
  if (millis() - led1 >= INTERVAL_LED1){
    led1 = millis();
    if(on1){
      on1 = false;
      digitalWrite(LED_PIN_1, HIGH);
    }else{
      on1 = true;
      digitalWrite(LED_PIN_1, LOW);
    }
  }
}

void blinkLED2(){
  if (millis() - led2 >= INTERVAL_LED2){
    led2 = millis();
    digitalWrite(LED_PIN_2, HIGH);
    if(on2){
      on2 = false;
      digitalWrite(LED_PIN_2, HIGH);
    }else{
      on2 = true;
      digitalWrite(LED_PIN_2, LOW);
    }
 }
}

void blinkLED3(){
  if (millis() - led3 >= INTERVAL_LED3){
    led3 = millis();
    digitalWrite(LED_PIN_3, HIGH);
    if(on3){
      on3 = false;
      digitalWrite(LED_PIN_3, HIGH);
    }else{
      on3 = true;
      digitalWrite(LED_PIN_3, LOW);
    }
  }
}


void loop() {
  float humidity = dht.readHumidity(); // 습도를 읽어온다 (백분율값)
  float temperature = dht.readTemperature(); // 온도를 읽어온다 (섭씨값)

  blinkLED1();
  blinkLED2();
  blinkLED3();

  while (Serial.available()){
    Serial.print("Input Your String (English or Number Only): ");
    String str = Serial.readString();
    lcd.setCursor(0, 1); // 0행 1열 부터
    lcd.print(str);
    Serial.println(str);
  }

  delay(100);
}