#include <Wire.h>

const int ACTON = 50;

const int ledPin   = PIN4; // optional
const int led2Pin  = PIN3; // optional
const int speaker  = PIN1;
const int MPU=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ;
double val;
bool woop = false;
int steep = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(speaker, OUTPUT);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  // on
  for (int i=127; i>5; i--) {
    analogWrite(speaker, i);
    analogWrite(ledPin, 255-(2*i));
    delay(15);
  }
}

void loop() {
  // measure because we may woop
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,6); // we need to read all 6 registers, really!

  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  //if (AcX <0) AcX*=-1;
  //if (AcY <0) AcY*=-1;
  //if (AcZ <0) AcZ*=-1;
  val = (AcX+AcY+AcZ)>>8; // shrink value

  if (woop == false) {
    if(val > ACTON) {
      woop = true;
      steep = 2;      
    } else {
      steep=0;
      analogWrite(speaker, 2); // no woop
      analogWrite(ledPin, 80);
      digitalWrite(led2Pin, LOW);
    }
  }
  
  if (woop) {
    steep++;
    if (steep > 50) {
      steep=45;
      if (val < 3) { // to slow movement? OFF
        woop=false;
      }
    } else {
      delay(10);
    }
    analogWrite(speaker, (int) steep);
    analogWrite(ledPin, 255);
    digitalWrite(led2Pin, HIGH);
  }
  
}

