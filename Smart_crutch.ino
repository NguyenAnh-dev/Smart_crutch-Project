
/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6GfO9t2Ck"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "PXP28OpBG6GRinQsCssuLluNLHSTs2r6"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <Wire.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "MAX30100_PulseOximeter.h"
#include <SoftwareSerial.h>
#include <WiFiManager.h>
BlynkTimer timer;
WiFiManager wifiManager;

#define ECHO1 32
#define TRIG1 33
#define ECHO2 25
#define TRIG2 26
#define ECHO3 27
#define TRIG3 13
#define in1 16
#define in2 17
#define btn1 18
#define btn2 19
#define coi 4
//#define nghieng 23

int btn1State = LOW;
int btn2State = LOW;

int lastBtn1State = LOW;
int lastBtn2State = LOW;

unsigned long btn1PressTime = 0;
unsigned long btn2PressTime = 0;
int Pos1, Pos2, Pos3;
const unsigned long longPressDuration = 1000, lastSendData = 0;

PulseOximeter pox;

float nhiptim, oxy;
float khoangcach = 0;




BLYNK_WRITE(V0) {
  Pos1 = param.asInt();
  vitri1();
}
BLYNK_WRITE(V1) {
  Pos2 = param.asInt();
  vitri2();
}
BLYNK_WRITE(V2) {
  Pos3 = param.asInt();
  vitri3();
}
void myTimerEvent() {
  Blynk.virtualWrite(V6, millis() / 1000);
}

void setup() {
  Serial.begin(9600);
Wire.begin();

  wifiManager.autoConnect("Nang chong thong minh", "66668888");
  Serial.println("connected... :)");
  Blynk.begin(BLYNK_AUTH_TOKEN, WiFi.SSID().c_str(), WiFi.psk().c_str());

  timer.setInterval(1000L, myTimerEvent);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  //pinMode(nghieng, INPUT);
  // put your setup code here, to run once:
  pinMode(ECHO1, INPUT);
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO3, INPUT);
  pinMode(TRIG3, OUTPUT);
  pinMode(coi, OUTPUT);

  if (!pox.begin()) {
    Serial.println("POX: FAILED");
    failed();
    for (;;)
      failed();
  } else {
    Serial.println("POX: SUCCESS");
  }
  // pox.setIRLedCurrent(MAX30100_LED_CURR_14_2MA);
  Wire.setClock(100000);
}

void loop() {
  Blynk.run();
  timer.run();
  // pox.update();
  btn1State = digitalRead(btn1);
  btn2State = digitalRead(btn2);

  if (btn1State != lastBtn1State) {
    if (btn1State == LOW) {
      btn1PressTime = millis();
    } else {
      if (millis() - btn1PressTime < longPressDuration) {
        dung();
      } else {
        len();
      }
    }
  }

  if (btn2State != lastBtn2State) {
    if (btn2State == LOW) {
      btn2PressTime = millis();
    } else {
      if (millis() - btn2PressTime < longPressDuration) {
        dung();
      } else {
        xuong();
      }
    }
  }




  if (millis() - lastSendData > 10000) {
    nhiptim = pox.getHeartRate();
    oxy = pox.getSpO2();


    Blynk.virtualWrite(V3, nhiptim);
    Blynk.virtualWrite(V4, oxy);

    Serial.print("Heart rate: ");
    Serial.print(nhiptim);
    Serial.print(" SpO2: ");
    Serial.print(oxy);
  }



  do_khoang_cach();
  lastBtn2State = btn2State;
  lastBtn1State = btn1State;
}



float get_distance1() {
  digitalWrite(TRIG1, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG1, LOW);
  float duration = pulseIn(ECHO1, HIGH);
  float distance = duration / 2 / 29.412;
  delay(100);
  return distance;
}
float get_distance2() {
  digitalWrite(TRIG2, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG2, LOW);
  float duration = pulseIn(ECHO2, HIGH);
  float distance = duration / 2 / 29.412;
  delay(100);
  return distance;
}
float get_distance3() {
  digitalWrite(TRIG3, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG3, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG3, LOW);
  float duration = pulseIn(ECHO3, HIGH);
  float distance = duration / 2 / 29.412;
  delay(100);
  return distance;
}

void failed() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}

void do_khoang_cach() {
  if (get_distance1() < 30 || get_distance2() < 30 || get_distance3() < 30) {  // || digitalRead(nghieng)

    //   if (digitalRead(nghieng) == HIGH ) {
    //     Blynk.logEvent("warning", "benh nhan bi nga");
    //   }
    //   canhbao(1);
    // } else {
    //   canhbao(0);
    // }



    Serial.print("sieu am1 :");
    Serial.print(get_distance1());
    Serial.print("\t sieu am2 :");
    Serial.print(get_distance2());
    Serial.print("\t sieu am3 :");
    Serial.println(get_distance3());
  }
}
  void len() {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    Serial.println("lennnnnnnnnnnnnnnnnnn");
  }

  void xuong() {
    digitalWrite(in2, HIGH);
    digitalWrite(in1, LOW);
    Serial.println("xuonggggggggggggggg");
  }
  void dung() {
    digitalWrite(in2, LOW);
    digitalWrite(in1, LOW);
    Serial.println("dungdddddddddddddd");
  }

  void canhbao(bool State) {
    digitalWrite(coi, State);
  }

  void vitri1() {
    len();
    delay(14000);

    xuong();
    delay(2000);
    dung();
    delay(200);
  }
  void vitri2() {
    len();
    delay(14000);
    xuong();
    delay(5000);
    dung();
    delay(200);
  }
  void vitri3() {
    len();
    delay(14000);
    xuong();
    delay(14000);
    dung();
    delay(200);
  }