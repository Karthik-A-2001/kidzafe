#include <SoftwareSerial.h>
#include<ArduinoJson.h>

SoftwareSerial gsm(2, 3);
String sendData;

void setup() {

  Serial.begin(9600);
  gsm.begin(9600);
  delay(1000);

  Serial.println("Checking ...");
  exec("AT+SAPBR=0,1");
  delay(1000);
  Serial.println(" ");
  exec("AT+HTTPTERM");
  delay(1000);
  Serial.println(" ");
  Serial.println("Executing ...");
  initGSM();
  initGPRS();
  exec("AT+CIFSR");
  Serial.println("HTTP Initialization ... ");
  delay(1000);
  initHTTP();
  Serial.println("HTTP POST ... ");
  postHTTP();


}



void loop() {
  if (gsm.available()) {
    Serial.write(gsm.read());
  }
  if (Serial.available()) {
    gsm.write(Serial.read());
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
}

void postHTTP() {
  connectGSM("AT+HTTPDATA=36,10000","DOWNLOAD");
  delay(2000);
  sendData = "{\"name\": \"Karthik\", \"domain\": \"GPS\"}";
  connectGSM(sendData,"OK");
  connectGSM("AT+HTTPACTION=1", "OK");
  delay(10000);
  exec("AT+HTTPREAD");
  connectGSM("AT+HTTPACTION=0", "OK");
  exec("AT+HTTPREAD");

  


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
  if (gsm.available() > 0)
    Serial.write(gsm.read());
}
