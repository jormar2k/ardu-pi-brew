
const int SS_WEBSERVER = 9;
const int SS_PID = 2;
const int CLK = 3;
const int MOSI_PID = 4;
const int analogTempPin = A5;

int integerTemp[8];
int pointTemp[8];
float binary[] = {128, 64, 32, 16, 8, 4, 2, 1};


long counter = 0;
double sensorValue = 0;
double meanSensorValue = 0;

void setup() {
  Serial.begin(115200);
  
  digitalWrite(SS_PID, HIGH);  // ensure SS stays high for now
  digitalWrite(CLK, LOW);
  digitalWrite(MOSI_PID, LOW);
  
  pinMode(CLK, OUTPUT);
  pinMode(MOSI_PID, OUTPUT);
  pinMode(SS_PID, OUTPUT);
}

void loop() {
  counter++;
  sensorValue = sensorValue + analogRead(analogTempPin);
  if (counter > 10000) {
    meanSensorValue = sensorValue / counter;

    double a = (840 - 667)/(80.1 - 62);
    double b = 667 - (a * 62);
  
    double temp = (meanSensorValue - b) / a;
    Serial.print(temp);
    Serial.print("  ");
    Serial.print(meanSensorValue);
    Serial.print("  ");
    Serial.println(analogRead(analogTempPin));
    createBinaryArrays(temp);
    
    sensorValue = 0;
    counter = 0;

    digitalWrite(MOSI_PID, LOW);
    digitalWrite(CLK, LOW);
    delay(50);
    digitalWrite(SS_PID, LOW);
    delay(20);
  
    sendBits(integerTemp);
    sendBits(pointTemp);
    
    digitalWrite(SS_PID, HIGH);
  }


}

void createBinaryArrays(double &temp) {
  createBinaryArray(integerTemp, temp);
  double roundedPoint = round(temp * 10);
  createBinaryArray(pointTemp, roundedPoint);
}

void createBinaryArray(int arr[], double &temp) {
    for (int i = 0; i < 8; i++) {
    if (temp >= binary[i]) {
      arr[i] = 1;
      temp -= binary[i];
    } else {
      arr[i] = 0;
    }
  }
}

void sendBits(int bits[]) {
   for (int i = 0; i < 8; i++) {
    digitalWrite(CLK, HIGH);
    digitalWrite(MOSI_PID, bits[i]);
    delay(10);
    digitalWrite(CLK, LOW);
    delay(10);
  }
}
