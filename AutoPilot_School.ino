#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#define IN1_L 3
#define IN2_L 2
#define IN3_L 1
#define IN4_L 0
#define IN1_R 7
#define IN2_R 6
#define IN3_R 5
#define IN4_R 4

HUSKYLENS huskylens;
SoftwareSerial mySerial(10, 11); // RX, TX
//HUSKYLENS green line >> Pin 10; blue line >> Pin 11
void printResult(HUSKYLENSResult result);

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);
    pinMode(IN1_L, OUTPUT);
    pinMode(IN2_L, OUTPUT);
    pinMode(IN3_L, OUTPUT);
    pinMode(IN4_L, OUTPUT);
    pinMode(IN1_R, OUTPUT);
    pinMode(IN2_R, OUTPUT);
    pinMode(IN3_R, OUTPUT);
    pinMode(IN4_R, OUTPUT);
    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
}

void loop() {
    if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    else
    {
        Serial.println(F("###########"));
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            int xdelta = result.xOrigin - result.xTarget;
            Serial.println(String()+F("Origin_axis: (")+result.xOrigin+F(", ")+result.yOrigin+F(") / Target_axis: (")+result.xTarget+F(", ")+result.yTarget+F(") ")+result.ID+F(" / xdiff: ")+xdelta);
            if(xdelta<0){
              digitalWrite(IN1_L, HIGH);
              digitalWrite(IN2_L, HIGH);
              digitalWrite(IN3_L, HIGH);
              digitalWrite(IN4_L, HIGH);
              digitalWrite(IN1_R, LOW);
              digitalWrite(IN2_R, LOW);
              digitalWrite(IN3_R, LOW);
              digitalWrite(IN4_R, LOW); //왼쪽 모터 활성화
            }
            else if(xdelta == 0){
              digitalWrite(IN1_L, HIGH);
              digitalWrite(IN2_L, HIGH);
              digitalWrite(IN3_L, HIGH);
              digitalWrite(IN4_L, HIGH);
              digitalWrite(IN1_R, HIGH);
              digitalWrite(IN2_R, HIGH);
              digitalWrite(IN3_R, HIGH);
              digitalWrite(IN4_R, HIGH); //둘 다 활성화
            }
            else if(xdela>0){
              digitalWrite(IN1_L, LOW;
              digitalWrite(IN2_L, LOW);
              digitalWrite(IN3_L, LOW);
              digitalWrite(IN4_L, LOW);
              digitalWrite(IN1_R, HIGH);
              digitalWrite(IN2_R, HIGH);
              digitalWrite(IN3_R, HIGH);
              digitalWrite(IN4_R, HIGH); //오른쪽 모터 활성화
            } 
            
        }    
    }
}
