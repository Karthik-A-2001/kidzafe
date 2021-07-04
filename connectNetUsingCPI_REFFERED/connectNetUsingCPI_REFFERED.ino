// Reference: https://circuitdigest.com/microcontroller-projects/send-data-to-web-server-using-sim900a-arduino

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX
String Start="AT+CIPSTART=\"TCP\"";     // TCPIP start command 
String ip="\"battlesnake-sl-2021.herokuapp.com/\"";  
int port=80;         
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  initGSM(); 
  initGPRS(); 
  //pinMode(13, OUTPUT);
  String svr=Start+","+ip+",\"80\"";
  delay(1000);
  exec(svr);
}

void loop() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}

void initGSM()
{
  connectGSM("AT","OK");
  connectGSM("ATE1","OK");
  connectGSM("AT+CPIN?","READY");
}
void initGPRS()
{
  connectGSM("AT+CIPSHUT","OK");
  connectGSM("AT+CGATT=1","OK");
  connectGSM("AT+CSTT=\"www\",\"\",\"\"","OK");
  exec("AT+CIICR");
  delay(1000);
  exec("AT+CIFSR");
  delay(1000);
  
}

void HTTPCheck()
{
  exec("at+httpinit");
  exec("at+httppara=\"CID\",1");
  exec("at+httppara=\"URL\",\"https://battlesnake-sl-2021.herokuapp.com/\"");
  exec("at+httpaction=0");
  delay(500);
  exec("at+httpterm");
}

void connectGSM (String cmd, char *res)
{
  while(1)
  {
    Serial.println(cmd);
    mySerial.println(cmd);
    delay(500);
    while(mySerial.available()>0)
    {
      if(mySerial.find(res))
      {
        delay(1000);
        return;
      }
    }
    delay(1000);
   }
 }

 void exec(String cmd)
 {
    Serial.println(cmd);
    mySerial.println(cmd);
 }
