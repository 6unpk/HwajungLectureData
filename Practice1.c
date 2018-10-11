#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#define DHTTYPE DHT11

/*
  실습1
  피에조 부저와 온습도 센서 실습
*/

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

// DHT 변수 - 온습도 센서를 제어한다
DHT dht(DHT_PIN, DHTTYPE);

// Setup - 처음 아두이노가 실행 될때의 부분 
void setup() {
  Wire.begin();
  // 시리얼 포트 Init
  Serial.begin(9600);
  // 온습도 센서 Init
  dht.begin();
  // 피에조 부저 Init
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(13, OUTPUT);
  // LED
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  on1 = on2 = on3 = false;
  led1 = led2 = led3 = millis();
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
    if(on3){
      on3 = false;
      digitalWrite(LED_PIN_3, HIGH);
    }else{
      on3 = true;
      digitalWrite(LED_PIN_3, LOW);
    }
  }
}

// loop - 반복 실행 되는 메인 루프
void loop() {
  float humidity = dht.readHumidity(); // 습도를 읽어온다 (백분율값)
  float temperature = dht.readTemperature(); // 온도를 읽어온다 (섭씨값)

  blinkLED1();
  blinkLED2();
  blinkLED3();

  Serial.println("Present humidity is " +(String) humidity );
  Serial.println("Present temperature is " + (String) temperature);

  delay(100);
}