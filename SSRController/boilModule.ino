// include the library code:
#include <BoilDisplay.h>
#include <EffectGauge.h>
#include <Rtd.h>


const int analogTempPin = A0;
const int ssrOut = 9;
const double percent = 100;
const int dutyCycle = 4000; //ms

BoilDisplay display(16, 2);
Rtd rtd(analogTempPin);

void setup() {
  Serial.begin(115200);
  pinMode(ssrOut, OUTPUT);

}

void loop() {

  double onTime = (percent/100)*dutyCycle;
  double offTime = dutyCycle - onTime;
  

  digitalWrite(ssrOut, HIGH);
  delay(onTime);
  digitalWrite(ssrOut, LOW);
  delay(offTime);
  
 

}
