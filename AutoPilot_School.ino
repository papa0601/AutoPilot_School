/* 라이브러리 불러오기 */
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

/* 모터 드라이버와 킬 스위치 포트 지정 */
#define DIR_L 2
#define PWM_L 3
#define DIR_R 4
#define PWM_R 5
#define BUTTON 13

/* 기본 속도와 회전 속도 및 지연 시간 지정 */
#define DEFAULTSPD 100
#define EXTRASPD 40
#define delayTime 100

/* 여러 회전 상황에 대한 기준 변수 지정 */
#define RCriteria 30
#define ACriteria 0
#define LOCriteria 60

/* 함수 원형 선언 */
void drive(int LSPEED, int RSPEED);
void rear();

HUSKYLENS huskylens;
SoftwareSerial mySerial(10, 11); // RX, TX
//HUSKYLENS green line >> Pin 10; blue line >> Pin 11
void setup() { // 기본 셋업
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
  int buttonState = digitalRead(BUTTON); // 킬 스위치 상태 받아옴
  /* 허스키 렌즈 에러에 대한 코드 */
  if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
  else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  else if(!huskylens.available()) { // 허스키 렌즈가 선을 못 잡으면 후진 
    Serial.print(F("No block or arrow appears on the screen!"));
    Serial.println(String()+F(" 후진"));
    if (buttonState == 0)
      rear();
    }
  else
  {
    Serial.println(F("###########"));
    /* 허스키 렌즈 오류 없을 때 */
    while (huskylens.available())
    {
      HUSKYLENSResult result = huskylens.read();
      int xdelta = result.xOrigin - result.xTarget; // 화살표(선)의 시작 지점 x값 - 끝 지점 x 값 (이후 '차이'라고 한다)
      int xavg = (result.xOrigin + result.xTarget) / 2; // 화살표(선)의 시작 지점 x값과 끝 지점 x값의 평균 (이후 '평균'이라고 한다)
      Serial.print(String()+F("Origin_axis: (")+result.xOrigin+F(", ")+result.yOrigin+F(") / Target_axis: (")+result.xTarget+F(", ")+result.yTarget+F(") ")+result.ID+F(" / xdiff: ")+xdelta);
      if (buttonState == 0) { // 버튼이 눌려있을 때
        if (xdelta <= -RCriteria && result.xTarget >= 160 + ACriteria || xavg >= 160 + LOCriteria){ // 차이가 회전 변수보다 크고 화살표 방향이 진행 뱡향과 다르면 또는 선이 우측에 몰리면
          Serial.println(String()+F("우회전"));
          drive(DEFAULTSPD + EXTRASPD, DEFAULTSPD);
          //turnR();
        }
        else if (xdelta >= RCriteria && result.xTarget <= 160 - ACriteria || xavg <= 160 - LOCriteria){ // 차이가 회전 변수보다 크고 화살표 방향이 진행 뱡향과 다르면 또는 선이 좌측에 몰리면
          Serial.println(String()+F("좌회전"));
          drive(DEFAULTSPD, DEFAULTSPD + EXTRASPD);
          //turnL();
        }
        else { // 둘 다 아니면 전진
          Serial.println(String()+F("전진"));
          drive(DEFAULTSPD, DEFAULTSPD);
          //forward();
        }
      }
      
      else { // 허스키 렌즈가 안되면 정지 (의미 X)
        Serial.println(String()+F("정지"));
        drive(0, -17);
        //stop();
      }
    }
  }
}

void drive(int LSPEED, int RSPEED) { // 주행 코드
  digitalWrite(DIR_L, LOW);
  digitalWrite(DIR_R, LOW);
  analogWrite(PWM_L, LSPEED);
  analogWrite(PWM_R, RSPEED+17);
  delay(delayTime);
}

void rear() { // 후진 코드
  digitalWrite(DIR_L, HIGH);
  digitalWrite(DIR_R, HIGH);
  analogWrite(PWM_L, 80);
  analogWrite(PWM_R, 80+17);
  delay(1000);
}
