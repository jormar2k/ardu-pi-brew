const int SS_WEBSERVER = 9;
const int SS_PID = 10;
const int CLK = 13;
const int MOSI_PID = 4;

int integerTemp[] = {0, 1, 0, 1, 0, 1, 1, 1};
int pointTemp[] = {0, 0, 0, 0, 1, 0, 0, 1};

void setup (void)
{ 
  Serial.begin(115200);

  digitalWrite(SS_PID, HIGH);  // ensure SS stays high for now
  digitalWrite(CLK, LOW);
  digitalWrite(MOSI_PID, LOW);
  
  pinMode(CLK, OUTPUT);
  pinMode(MOSI_PID, OUTPUT);
  pinMode(SS_PID, OUTPUT);

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  
}


void loop (void)
{

  digitalWrite(MOSI_PID, LOW);
  digitalWrite(CLK, LOW);
  delay(50);
  digitalWrite(SS_PID, LOW);
  delay(20);

  sendBits(integerTemp);
  sendBits(pointTemp);
  
  digitalWrite(SS_PID, HIGH);

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

