#include <SPI.h>
#include <Rtd.h>


const int analogTempPin = A1;
const int SS_WEBSERVER = 10;

//SPISettings settings(1000, MSBFIRST, SPI_MODE0);
Rtd rtd(analogTempPin);

long counter = 0;
double sensorValue = 0;
double meanSensorValue = 0;

void setup() {
  Serial.begin(115200);

  digitalWrite(SS, HIGH);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV128);
}

void loop() {
  counter++;

  sensorValue = sensorValue + analogRead(analogTempPin);
  if (counter > 10000) {
    meanSensorValue = sensorValue / counter;

    double a = (840 - 667)/(80.1 - 62);
    double b = 667 - (a * 62);
  
    double temp = (meanSensorValue - b) / a;

    digitalWrite(SS, LOW);
    SPI.transfer(87);
    digitalWrite(SS, HIGH);
    delay(500);

    Serial.print(a, 6);
    Serial.print(" * ");
    Serial.print(meanSensorValue);
    Serial.print(" + ");
    Serial.print(b, 6);
    Serial.print(" = ");
    Serial.println(temp, 1);

    sensorValue = 0;
    counter = 0;
  }
 
}
