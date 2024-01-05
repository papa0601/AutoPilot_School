#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#define DIR_L 2
#define PWM_L 3
#define DIR_R 4
#define PWM_R 5
#define BUTTON 13

//#define FSPEED 100
//#define RSPEED 60

#define DEFAULTSPD 100
#define EXTRASPD 40
#define delayTime 100

#define RCriteria 30
#define ACriteria 0
#define LOCriteria 60

//void forward();
//void stop();
//void turnR();
//void turnL();

void drive(int LSPEED, int RSPEED);
void rear();

HUSKYLENS huskylens;
SoftwareSerial mySerial(10, 11); // RX, TX
//HUSKYLENS green line >> Pin 10; blue line >> Pin 11
void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);
    pinMode(DIR_L, OUTPUT);
    pinMode(PWM_L, OUTPUT);
    pinMode(DIR_R, OUTPUT);
    pinMode(PWM_R, OUTPUT);
    pinMode(BUTTON, INPUT);
    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
}

void loop() {
  int buttonState = digitalRead(BUTTON);
  if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
  else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  else if(!huskylens.available()) {
    Serial.print(F("No block or arrow appears on the screen!"));
    Serial.println(String()+F(" 후진"));
    if (buttonState == 0)
      rear();
    }
  else
  {
    Serial.println(F("###########"));
    while (huskylens.available())
    {
      HUSKYLENSResult result = huskylens.read();
      int xdelta = result.xOrigin - result.xTarget;
      int xavg = (result.xOrigin + result.xTarget) / 2;
      Serial.print(String()+F("Origin_axis: (")+result.xOrigin+F(", ")+result.yOrigin+F(") / Target_axis: (")+result.xTarget+F(", ")+result.yTarget+F(") ")+result.ID+F(" / xdiff: ")+xdelta);
      if (buttonState == 0) { // 버튼 눌렀을 때
        if (xdelta <= -RCriteria && result.xTarget >= 160 + ACriteria || xavg >= 160 + LOCriteria){ // 우측 ㄱㄱ
          Serial.println(String()+F("우회전"));
          drive(DEFAULTSPD + EXTRASPD, DEFAULTSPD);
          //turnR();
        }
        else if (xdelta >= RCriteria && result.xTarget <= 160 - ACriteria || xavg <= 160 - LOCriteria){ // 좌측 ㄱㄱ
          Serial.println(String()+F("좌회전"));
          drive(DEFAULTSPD, DEFAULTSPD + EXTRASPD);
          //turnL();
        }
        else { // 앞으로
          Serial.println(String()+F("전진"));
          drive(DEFAULTSPD, DEFAULTSPD);
          //forward();
        }
      }
      
      else {
        Serial.println(String()+F("정지"));
        drive(0, -17);
        //stop();
      }
    }
  }
}

void drive(int LSPEED, int RSPEED) {
  digitalWrite(DIR_L, LOW);
  digitalWrite(DIR_R, LOW);
  analogWrite(PWM_L, LSPEED);
  analogWrite(PWM_R, RSPEED+17);
  delay(delayTime);
}

void rear() {
  digitalWrite(DIR_L, HIGH);
  digitalWrite(DIR_R, HIGH);
  analogWrite(PWM_L, 80);
  analogWrite(PWM_R, 80+17);
  delay(1000);
}

/*
void forward() {
  digitalWrite(DIR_L, LOW); // LOW -> 전진 
  analogWrite(PWM_L, FSPEED);
  digitalWrite(DIR_R, LOW); // LOW -> 전진
  analogWrite(PWM_R, FSPEED);
  delay(delayTime);
}

void stop() {
  analogWrite(PWM_L, 0);
  analogWrite(PWM_R, 0);
  delay(delayTime);
}

void turnR() {
  digitalWrite(DIR_L, LOW); // LOW -> 전진 
  analogWrite(PWM_L, RSPEED);
  digitalWrite(DIR_R, HIGH); // LOW -> 전진
  analogWrite(PWM_R, RSPEED);
  delay(delayTime);
}

void turnL() {
  digitalWrite(DIR_L, HIGH); // LOW -> 전진 
  analogWrite(PWM_L, RSPEED);
  digitalWrite(DIR_R, LOW); // LOW -> 전진
  analogWrite(PWM_R, RSPEED);
  delay(delayTime);
}
*/


