#include <Servo.h>
#include <Wire.h>
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C TV(0x27, 16, 2);  // TODO: I2C LCD를 TV이름으로 선언

int piezoPin = 2;          //피에조부저를 오렌지보드 D2번에 연결
int irPin = A1;            //적외선장애물센서를 오렌지보드 A1번에 연결

bool flag = 1;



int CdS_pin = A0;


// LED pins
int ledR1 = 10;
int ledG1 = 9;
int ledB1 = 8;
int ledR2 = 13;
int ledG2 = 12;
int ledB2 = 11;


bool light_status = 0;
int threshold = 350;


int servo_pin = 6;

Servo my_servo;


void setup() {
  // put your setup code here, to run once:
  for(int i = 8; i < 14; i++)
  {
    pinMode(i, OUTPUT);
  }
  Serial.begin(9600);

  my_servo.attach(servo_pin);

  TV.init();
  TV.backlight();
  TV.setCursor(0, 0);
  TV.print("Looking TV....");

}

void loop() {
  // put your main code here, to run repeatedly:

  int IRval = analogRead(irPin);    //적외선장애물센서 측정

  if (IRval < 100) {          // TODO: TV가 켜져있고 거리가 가깝다면 TV를 끈다
    if (flag == 1) {
      flag = 0;
      TV.clear();
      TV.noBacklight();
    }
    tone(2, 1000, 300);       // TODO: 거리가 가깝다면 피에조부저에서 0.3초 간격으로 알람이 울린다
    delay(600);
  }

  else if (IRval > 900 && flag == 0) {  // TODO: TV가 꺼져있고 거리가 멀어지면 TV를 켠다
    flag = 1;
    TV.backlight();
    TV.print("Looking TV....");
    noTone(2);
  }
  delay(100);

  int CdS_value = analogRead(CdS_pin);
  Serial.print("CdS value: ");
  Serial.println(CdS_value);

  if (CdS_value < threshold && light_status) // bright
  {
    light_status = 0;
    digitalWrite(ledR1, LOW);
    digitalWrite(ledG1, LOW);
    digitalWrite(ledB1, LOW);

    digitalWrite(ledR2, LOW);
    digitalWrite(ledG2, LOW);
    digitalWrite(ledB2, LOW);

    my_servo.write(150); // TODO
    delay(1000);
  }

  else if (CdS_value >= threshold && !light_status) // bright
  {
    light_status = 1;
    digitalWrite(ledR1, HIGH);
    digitalWrite(ledG1, HIGH);
    digitalWrite(ledB1, HIGH);

    digitalWrite(ledR2, HIGH);
    digitalWrite(ledG2, HIGH);
    digitalWrite(ledB2, HIGH);

    my_servo.write(-30); // TODO
    delay(1000);
  }



  delay(500);




}
