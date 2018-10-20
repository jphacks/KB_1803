#include <WiFi.h>
#include <WiFiServer.h>
const char* ssid = "yodai";
//const char* ssid = "LiveMechX-316";
//const char* ssid = "F186";
const char* password = "Youdai7019284";
//const char* password = "mlab0423887074";
char server[] = "smtp.mail.yahoo.co.jp";

const int motor_pin = 2;
const int temp_pin = 4;
const int wifi_pin = 16;
const int threshold = 10; //0-100間での任意の値

byte sendEmail(void);
byte eRcv(void);
int IDSread(void);
int AnaToCm(int);
WiFiClient client;
int i = 0;

void setup()
{ pinMode(motor_pin, OUTPUT);
  pinMode(temp_pin, INPUT);
  pinMode(wifi_pin, INPUT);
  
  Serial.begin(115200);
  delay(10);
  Serial.println("");
  Serial.println("");
  Serial.print("Connecting To ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop()
{
 int val ;
 val = IDSread()  ;  // read templature

 if(!val) i = 0;
 if(val) i++;
Serial.println(i);
 if(i > 0){
    digitalWrite(motor_pin,HIGH);
    byte ret = sendEmail();
    WiFi.disconnect();
    while(1){
      delay(100);
    }
  }
 delay(1000);
}

byte sendEmail()
{
  byte thisByte = 0;
  byte respCode;

  if (client.connect(server, 587) == 1) {
    Serial.println(F("connected"));
  } else {
    Serial.println(F("connection failed"));
    return 0;
  }
  if (!eRcv()) return 0;

  Serial.println(F("Sending EHLO"));
  client.println("EHLO www.example.com");
  if (!eRcv()) return 0;
  Serial.println(F("Sending auth login"));
  client.println("auth login");
  if (!eRcv()) return 0;
  Serial.println(F("Sending User"));
  // Change to your base64, ASCII encoded user
  client.println("bGl2ZV9tZWNoX2VuZw=="); //<---------User
  if (!eRcv()) return 0;
  Serial.println(F("Sending Password"));
  // change to your base64, ASCII encoded password
  client.println("bWxhYjA0MjM4ODcwNzQ=");//<---------Passw
  if (!eRcv()) return 0;
  Serial.println(F("Sending From"));
  // change to your email address (sender)
  client.println(F("MAIL From: live_mech_eng@yahoo.co.jp"));
  if (!eRcv()) return 0;
  // change to recipient address
  Serial.println(F("Sending To"));
  //client.println(F("RCPT To: y.hirano@live.mech.eng.osaka-u.ac.jp"));
  client.println(F("RCPT To: hiroakikidd@yahoo.co.jp"));
  if (!eRcv()) return 0;
  Serial.println(F("Sending DATA"));
  client.println(F("DATA"));
  if (!eRcv()) return 0;
  Serial.println(F("Sending email"));
  // change to recipient address
  //client.println(F("To: y.hirano@live.mech.eng.osaka-u.ac.jp"));
  client.println(F("To: hiroakikidd@yahoo.co.jp"));
  // change to your address
  client.println(F("From: live_mech_eng@yahoo.co.jp"));
  client.println(F("Subject: Warning!! \r\n"));
  client.println(F("User temprature is rising...\r\n"));
  client.println(F("Please check the status.\r\n"));

  client.println(F("."));
  if (!eRcv()) return 0;
  Serial.println(F("Sending QUIT"));
  client.println(F("QUIT"));
  if (!eRcv()) return 0;
  client.stop();
  Serial.println(F("disconnected"));
  return 1;
}

byte eRcv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;

  while (!client.available()) {
    delay(1);
    loopCount++;
    // if nothing received for 10 seconds, timeout
    if (loopCount > 10000) {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }

  respCode = client.peek();
  while (client.available())
  {
    thisByte = client.read();
    Serial.write(thisByte);
  }

  if (respCode >= '4')
  {
    //  efail();
    return 0;
  }
  return 1;
}

int IDSread() {
     int i = 0;
     int j = 0;
     int ans = 0;

     while(i<100){
        i++;
        if(digitalRead(temp_pin) == LOW) j++;
        delay(100);
        Serial.println(j);
     }

     if(j < threshold) ans = 0;
     if(j >= threshold){
     ans = 1;
     }
     delay(2000);
     return ans;
}

