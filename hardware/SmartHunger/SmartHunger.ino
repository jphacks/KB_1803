#include <EspBLE.h>
#include <WiFi.h>
#include <WiFiServer.h>
#include <time.h>
#include <ArduinoJson.h>
#include <SparkFunCCS811.h>
#include <SI7021.h>


/************************************
          BLE config
*************************************/
EspBLE esp;

const uint8_t SERVICE_UUID[16] = {
    0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x07, 0x08,
    0x09, 0x0a, 0x0b, 0x0c,
    0x0d, 0x0e, 0x0f, 0x10
};

const uint8_t CHAR_UUID[16] = {
    0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x07, 0x08,
    0x09, 0x0a, 0x0b, 0x0c,
    0x0d, 0x0e, 0x0f, 0x10
};


/************************************
          Wi-Fi config
*************************************/
//String WIFI_SSID = "~(=^･ω･^)";
//String WIFI_PASSWORD = "nyannyan";
String WIFI_SSID = "500KOBE";
String WIFI_PASSWORD = "@KIITO2018";
#define JST   3600*9


/************************************
          Access to Server
*************************************/
WiFiClient client;
const char* host = "nanocon.herokuapp.com";
unsigned int port = 80;


/************************************

/************************************
         Analyzed IR code
*************************************/
String IR_NUMBER = "0000";


/************************************
         WEet analyze
*************************************/
int WET_val = 0;
char WET_c[6]; 


/************************************
      CO2/TVOC sensor module
*************************************/
#define CCS811_ADDR 0x5B //Default I2C Address
CCS811 mySensor(CCS811_ADDR);
char CO2_c[10];
char TVOC_c[10];
SI7021 sensor;


/************************************
              IFTTT
    Webhooks Connection marker
*************************************/
String makerEvent = "Trillion_LINE_notification"; // Maker Webhooks
String makerKey = "b9AGCbD5-0QcL1Kb4qLO5d"; // Maker Webhooks
const char* server = "maker.ifttt.com";  // Server URL


#include "wifiset.h"
#include "alert.h"
#include "ble.h"
#include "port.h"



void setup(){
  Serial.begin(115200);
  while(!Serial);
  Serial.println("START");
  wifi_init();

  //---------------------------
  // BLE start
  //---------------------------
  esp.setServiceUUID(SERVICE_UUID, sizeof(SERVICE_UUID));
  esp.setCharUUID(CHAR_UUID, sizeof(CHAR_UUID));
  esp.init();
  Serial.println("BLE START");

  //---------------------------
  // CCS811 : CO2, TVOC
  //---------------------------
  mySensor.begin();
  Serial.println("CO2,TVOC START");

}



void loop(){
  //---------------------------
  // Time adjust
  //---------------------------
  if(WiFi.status() == WL_CONNECTED){
    make_data();
  }


  //---------------------------
  // Wi-Fi update (from BLE)
  //---------------------------
  String str = ble_get_data();
  if(str.indexOf("SSID:") == 0){
    WIFI_SSID = str;
    wifi_init();
  }
  else if(str.indexOf("PASSWORD:") == 0){
    WIFI_PASSWORD = str;
    wifi_init();
  }
  else if(str.indexOf("IR:") == 0){
    IR_NUMBER = str;
  }
  Serial.print(WIFI_SSID + "//");
  Serial.println(WIFI_PASSWORD);
  Serial.println("-------------------------------------------------");

  
  //---------------------------
  // Wet voltage 
  //---------------------------
  for(int i = 0; i < 100; i++){
    WET_val += analogRead(A7);
    delay(10);
  }
  WET_val /= 100;
  memset(WET_c, '\0', sizeof(WET_c));
  sprintf(WET_c, "%d", WET_val);
  Serial.print("wet:");
  Serial.println(WET_c);
  WET_val = 0;


  //---------------------------
  // get from CO2 sensor 
  //---------------------------
  mySensor.readAlgorithmResults();
  memset(CO2_c, '\0', sizeof(CO2_c));
  memset(TVOC_c, '\0', sizeof(TVOC_c));
  sprintf(CO2_c, "%d", (int)mySensor.getCO2());
  sprintf(TVOC_c, "%d", (int)mySensor.getTVOC());
  Serial.print("co2:");
  Serial.println(CO2_c);
  Serial.print("tvoc:");
  Serial.println(TVOC_c);

  
  delay(1000);
}




