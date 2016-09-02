#include <PID_v1.h>
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

//Define Variables we'll be connecting to
double setpoint = 55;
double output;
unsigned long now;
double consKp=500, consKi=0.0000000001, consKd=0;
double aggKp=2*consKp, aggKi=2*consKi, aggKd=0;
int windowSize = 5000;
unsigned long windowStartTime;

const int TWO_MINUTES = 30000;
const int analogTempPin = A0;
const int relayPin = 9;
double temp = 0;

PID myPID(&temp, &output, &setpoint, consKp, consKi, consKd, DIRECT);

void setup() {
  pinMode(nSS, INPUT);
  pinMode(MOSI_HLT, INPUT);
  pinMode(CLK, INPUT);
  
  Serial.begin(115200);
   //initialize the variables we're linked to
  windowStartTime = millis();
  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, windowSize);
  //turn the PID on
  myPID.SetMode(MANUAL);
  output = 270;

}

void loop() {
  readSpi();
  doPid();

  if (millis() > TWO_MINUTES) {
    myPID.SetMode(AUTOMATIC);
  }

}

void doPid() {
  //double gap = abs(setpoint - temp);
  //if(gap < 10)
  //{
    //myPID.SetTunings(consKp, consKi, consKd);
  //}
  //else
  //{
     //myPID.SetTunings(aggKp, aggKi, aggKd);
  //}
  
  myPID.Compute();
  
  now = millis();
  if(now - windowStartTime > windowSize) {
    windowStartTime += windowSize;
  }

  if((output > now - windowStartTime) && output > 100) {
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }
}

void readSpi() {
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
      temp = (pointValue * 0.1) + integerValue;
      Serial.print(temp);
      Serial.print("    ");
      Serial.print(output);
      Serial.print("    ");
      Serial.println(millis());
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
