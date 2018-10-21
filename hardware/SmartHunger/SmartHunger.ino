#include <EspBLE.h>
#include <WiFi.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <time.h>
#include <SparkFunCCS811.h>
#include <SI7021.h>


#include "IR.h"
#include "sensor.h"
#include "wifiset.h"
#include "ble.h"
#include "port.h"


void setup() {
  //---------------------------
  // Serial start
  //---------------------------
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Serial START");


  //---------------------------
  // WiFi start
  //---------------------------
  wifi_init();
  Serial.println("WiFi START");


  //---------------------------
  // BLE start
  //---------------------------
  ble_init();
  Serial.println("BLE START");


  //---------------------------
  // Sensor start
  //---------------------------
  sensor_init();
  Serial.println("Sensor START");


  //---------------------------
  // Now time setting
  //---------------------------
  get_time();
  NOW = MIN;


  //---------------------------
  // IR init
  //---------------------------
  IR_init();

  Serial.println("=================================");
}



void loop() {
  //---------------------------
  // Time get
  //---------------------------
  get_time();
  Serial.println(MIN);
  Serial.println(NOW);
  //Serial.println(SEC);


  //---------------------------
  // WiFi adjust
  //---------------------------
  wifi_update();

  //  if(SEC == 0 || SEC % 20 == 0){
  //    getWet();
  //    getAir();
  //    getEnv();
  //
  //    if (WiFi.status() == WL_CONNECTED) {
  //      Serial.println("=================================");
  //      Serial.println("send data");
  //      send_data();
  //    }
  //  }

  if (MIN > NOW || (NOW == 59 && MIN == 0)) {
    NOW = MIN;


    //---------------------------
    // get Sensor data
    //---------------------------
    getWet();
    getAir();
    getEnv();


    //---------------------------
    // Post data for server
    //---------------------------


    if (WET_val < 100) {
      for (int i = 0; i < 20 ; i++) {
        IR_send();
      }
    }
  }

  Serial.println("=================================");
  delay(1000);
}




