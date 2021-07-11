#include <SoftwareSerial.h>
#include<ArduinoJson.h>
#include <TinyGPS.h>

TinyGPS gps;

SoftwareSerial gsm(2, 3);
SoftwareSerial gpsSerial(6, 7);

String sendData;
String Location;
float gpslat, gpslon;
int state = 0;
const int pin = 12;
int led = 13;
int po = 11;
int buttonState = 0;
void setup() {

  Serial.begin(9600);
  gsm.begin(9600);
  gpsSerial.begin(9600);
  delay(1000);
  pinMode(po, OUTPUT);
  pinMode(pin, INPUT);
  pinMode(led, OUTPUT);
  Serial.println("Checking ...");
  exec("AT+SAPBR=0,1");
  delay(1000);
  Serial.println(" ");
  exec("AT+HTTPTERM");
  delay(1000);
  Serial.println(" ");
  Serial.println("Executing ...");
     gsm.listen();
  initGSM();
  initGPRS();
  exec("AT+CIFSR");
  Serial.println("HTTP Initialization ... ");
  delay(1000);
  initHTTP();

}



void loop() {
  digitalWrite(po, HIGH);
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);
  buttonState = digitalRead(pin);
  gpsSerial.listen();
  while (gpsSerial.available()) {
    int c = gpsSerial.read();
    if (gps.encode(c))
    {
      gps.f_get_position(&gpslat, &gpslon);
    }
  }
  Location = "https://www.google.com/maps/?q=" + String(gpslat, 6) + "," + String(gpslon, 6);
  sendData = "{\"name\": \"Karthik\", \"domain\": \"" + Location + "\"}";
  if (voltage > 4) {
    digitalWrite(led, HIGH);
    gsm.listen();
    Serial.println("HTTP POST ... ");
    postHTTP();
    Serial.println("HTTP GET ... ");
    getHTTP();
    if (gsm.available() > 0)
      Serial.write(gsm.read());
  }
  else {
    digitalWrite(led, LOW);
  }
}



void initGSM() {
  connectGSM("AT", "OK");
  connectGSM("ATE1", "OK");
  connectGSM("AT+CPIN?", "READY");
}

void initGPRS() {
  connectGSM("AT+CIPSHUT", "OK");
  connectGSM("AT+CGATT=1", "OK");
  connectGSM("AT+CSTT=\"www\",\"\",\"\"", "OK");
  connectGSM("AT+CIICR", "OK");
}

void initHTTP() {
  connectGSM("AT+SAPBR=3,1,\"APN\",\"CMNET\"", "OK");
  connectGSM("AT+SAPBR=1,1", "OK");
  connectGSM("AT+HTTPINIT", "OK");
  connectGSM("AT+HTTPPARA=\"CID\",1", "OK");
  connectGSM("AT+HTTPPARA= \"URL\",\"kidzafe.herokuapp.com/persons\"", "OK");
}

void getHTTP() {
  connectGSM("AT+HTTPACTION=0", "OK");
  delay(15000);
  exec("AT+HTTPREAD");
  if (gsm.available() > 0)
    Serial.write(gsm.read());
}

void postHTTP() {
  connectGSM("AT+HTTPDATA=83,10000", "DOWNLOAD");
  delay(3000);

  connectGSM(sendData, "OK");
  delay(1000);
  connectGSM("AT+HTTPACTION=1", "OK");
  delay(10000);
}

void connectGSM (String cmd, char* res)
{
  while (1)
  {
    Serial.println(cmd);
    gsm.println(cmd);
    delay(1000);
    while (gsm.available() > 0)
    {
      if (gsm.find(res))
      {
        delay(1000);
        return;
      }
    }
    delay(1000);
  }
  if (gsm.available() > 0)
    Serial.write(gsm.read());
}

void exec(String s) {
  Serial.println(s);
  gsm.println(s);
  delay(3000);
  if (gsm.available())
    Serial.write(gsm.read());
}
