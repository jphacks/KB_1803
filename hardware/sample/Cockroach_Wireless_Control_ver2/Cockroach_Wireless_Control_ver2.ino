#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
 
#define RIGHT 4
#define LEFT 5

void STIMULATE_RIGHT();
void STIMULATE_LEFT();
 
Ticker ticker;
 
const char* ssid = "ESP8266AP2";
const char* password = "password";
 
ESP8266WebServer server(80);
 
enum  stimulation_style {stimulation_blank = 0, stimulation_right = 1, stimulation_left = 2} ;
static enum stimulation_style Stimulation = stimulation_blank;
long tick_counter_1 = 0;
long tick_counter_2 = 0;

void ticker_func() {
  static byte right_state = 0;
  static byte left_state = 0;
  tick_counter_1++;
  tick_counter_2++;
    
  if ((Stimulation == stimulation_blank) || 
    (Stimulation == stimulation_left && (tick_counter_1 % 10 != 0)) ||  
    (Stimulation == stimulation_right && (tick_counter_2 % 10 != 0))){
    right_state = 0;
    left_state = 0;
  }
    
  else if  (Stimulation == stimulation_left && (tick_counter_1 % 10 == 0)){
    right_state = 0;
    left_state = 1;
  }

  else if  (Stimulation == stimulation_right && (tick_counter_2 % 10 == 0)){
    right_state = 1;
    left_state = 0;
  } 

  digitalWrite(RIGHT, right_state);
  digitalWrite(LEFT, left_state);
}

static const char* cpResponse400 = "<HTML><BODY>Bad request</BODY></HTML>\r\n";
static const char* cpResponse200 = "<HTML><BODY style='font-size:60px;'><div class=demo demo10><div class=heading><div Align=center><span>Stimulation Control</span></div></div></div><br/><br/><br/>"
 "<input type=button value=OFF onclick=location.href='/cmd?STIMULATION=blank' style=font-size:60px;font-weight:bold;border:medium solid #191970;background:#00008b;color:ffff00;><br/><input type=button value=RIGHT onclick=location.href='/cmd?STIMULATION=RIGHT' style=font-size:60px;font-weight:bold;border:medium solid #191970;background:#00008b;color:ffff00;><br/><input type=button value=LEFT onclick=location.href='/cmd?STIMULATION=LEFT' style=font-size:60px;font-weight:bold;border:medium solid #191970;background:#00008b;color:ffff00;><br/>"
 "</BODY></HTML>\r\n";
 
void send_bad_request() {
  server.send(400, "text/html", cpResponse400);
  Serial.println("Bad request");
}

void handleCommand() {
  if (!server.hasArg("STIMULATION")) {
      send_bad_request();
      return;
  }
  String cmd = server.arg("STIMULATION");
  Serial.println("handleCommand() STIMULATION=" + cmd);
  if (cmd == "blank")  
    Stimulation = stimulation_blank;
    
  else if (cmd == "RIGHT")
    Stimulation = stimulation_right;
    
  else if (cmd == "LEFT")
    Stimulation = stimulation_left;
    
  server.send(200, "text/html", cpResponse200);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(RIGHT, OUTPUT);
  pinMode(LEFT, OUTPUT);
  digitalWrite(RIGHT, 0);
  digitalWrite(LEFT, 0);
  ticker.attach_ms(2, ticker_func);

  WiFi.softAP(ssid, password);
  IPAddress ip = WiFi.softAPIP();
  Serial.println("");
  Serial.println(ssid + String(" starts..")); 
  Serial.print("this AP : "); 
  Serial.println(ip);
 
  server.on("/cmd", handleCommand);
  server.begin();
  Serial.println("HTTP server started");
}


void loop() { 
  server.handleClient();
  delay(1);
}



