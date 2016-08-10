// include the library code:
#include <EffectGauge.h>
#include <Rtd.h>
#include <PID_v1.h>

//Define Variables we'll be connecting to
double setpoint = 85;
double output;
unsigned long now;
double aggKp=4, aggKi=0.2, aggKd=1;
double consKp=1, consKi=0.05, consKd=0.25;
int windowSize = 5000;
unsigned long windowStartTime;

const int analogTempPin = A0;
const int relayPin = 9;
int counter = 0;
double sensorValue = 0;
double meanSensorValue = 0;
double temp = 0;

Rtd rtd(analogTempPin);
PID myPID(&temp, &output, &setpoint, consKp, consKi, consKd, DIRECT);

void setup() {
  Serial.begin(115200);
   //initialize the variables we're linked to
  windowStartTime = millis();
  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, windowSize);
  //turn the PID on
  myPID.SetMode(AUTOMATIC);

}

void loop() {
  counter++;

  sensorValue = sensorValue + analogRead(analogTempPin);
  if (counter > 10000) {
    meanSensorValue = sensorValue / counter;
    temp = rtd.getTemperature(meanSensorValue);

    Serial.print(output);
    Serial.print("       ");
    Serial.print(setpoint);
    Serial.print("       ");
    Serial.println(temp);
    
    sensorValue = 0;
    counter = 0;
  }

  double gap = abs(setpoint - temp);
  if(gap < 10)
  {
    myPID.SetTunings(consKp, consKi, consKd);
  }
  else
  {
     myPID.SetTunings(aggKp, aggKi, aggKd);
  }
  
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
