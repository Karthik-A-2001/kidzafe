#include <SoftwareSerial.h>

SoftwareSerial gsm(2, 3); // RX, TX

// gsm -> GSM Module
// Serial -> Arduino(reads onto Serial Monitor)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gsm.begin(9600);
  initGSM();
  initGPRS();
  // HTTPCheck(); // Throwing some buggy results
  exec("AT+CIPSTART = \"TCP\",\"battlesnake-sl-2021.herokuapp.com\",\"80\"");
  // exec("AT+CIPSTART = \"TCP\",\"matuluka.tmep.cz\",\"80\"");
}

/*
 * 
 * Serial Monitor Seetings: Both NL & CR
 * 
 * Command List:
 AT+CIPSTART = "TCP","battlesnake-sl-2021.herokuapp.com","80"
 AT+CIPSEND=60
 >
 GET https://battlesnake-sl-2021.herokuapp.com/ HTTP/1.0 \r\n
 * 
*/

void loop() {
  // put your main code here, to run repeatedly:
   if (gsm.available()) {
    Serial.write(gsm.read());
  }
  if (Serial.available()) {
    gsm.write(Serial.read());
  }
}

void initGSM() {
  connectGSM("AT","OK");
  connectGSM("ATE1","OK");
  connectGSM("AT+CPIN?","READY");
}

void initGPRS() {
  connectGSM("AT+CIPSHUT","OK");
  connectGSM("AT+CGATT=1","OK");
  connectGSM("AT+CSTT=\"www\",\"\",\"\"","OK");
  connectGSM("AT+CIICR","OK");
  exec("AT+CIFSR");
}

void HTTPCheck()
{
  exec("AT+HTTPINIT");
  exec("AT+HTTPPARA=\"CID\",1");
  exec("AT+HTTPPARA=\"URL\",\"https://battlesnake-sl-2021.herokuapp.com/\"");
  exec("AT+HTTPACTION=0");
  exec("AT+HTTPTERM");
}

void exec(String s) {
  Serial.println(s);
  gsm.println(s);
  delay(1000);
  if(gsm.available()>0) {
    Serial.write(gsm.read());
  }
}

// available -> check if content ready to read
// write -> binary format on Serial port
// println -> ASCII format on Serial port
// find -> compare against expected result

void connectGSM (String cmd, char* res)
{
  while(1)
  {
    Serial.println(cmd);
    gsm.println(cmd);
    delay(500);
    while(gsm.available()>0)
    {
      if(gsm.find(res))
      {
        delay(1000);
        return;
      }
    }
    delay(1000);
   }
 }
