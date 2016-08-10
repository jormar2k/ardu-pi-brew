#include <math.h>

const int nSS = 10;
const int MOSI_HLT = 4;
const int CLK = 13;

int lastState = 0;
int nowState = 0;
int counter = 0;
boolean fallingEdge = false;
boolean reading = false;

const int shiftRegisterLength = 16;
int shiftRegister[shiftRegisterLength];


void setup() {
  pinMode(nSS, INPUT);
  pinMode(MOSI_HLT, INPUT);
  pinMode(CLK, INPUT);
  Serial.begin(115200);
}

void loop() {
  if (digitalRead(nSS) == 0) {
    reading = false;
    
    nowState = digitalRead(CLK);
    if (nowState == 0) {
      if (lastState == 1) {
        fallingEdge = true;
        lastState = nowState;
      } else {
        fallingEdge = false;
      }
    } else {
        lastState = nowState;
    }

    if (fallingEdge) {
      int bitValue = digitalRead(MOSI_HLT);
      shiftRegister[counter] = bitValue;
      counter++;
    }
  } else {
    if (!reading) {
      reading = true;
      counter = 0;
      float integerValue = readBits(0);
      float pointValue = readBits(8);
      float tempValue = (pointValue * 0.1) + integerValue;
      Serial.println(tempValue);
    }

  }
}

float readBits(int offset) {
  float value = 0;
  for (int i = offset; i < offset + 8; i++) {
    if (shiftRegister[i] == 1) {
      float poop = i + 1;
      value += pow(2, (offset + 8) - poop);
    }
  }

  return round(value);
}

