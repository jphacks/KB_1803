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
        BLE initialization
*************************************/

void ble_init(){
    esp.setServiceUUID(SERVICE_UUID, sizeof(SERVICE_UUID));
    esp.setCharUUID(CHAR_UUID, sizeof(CHAR_UUID));
    esp.init();
}

/************************************
        BLE data recieve
*************************************/
String ble_get_data(){
  //Receive Data from Client
  uint8_t c;
  uint8_t len;
  String str = "";
  if(len = esp.available()){
    Serial.print("Detect Write Request: ");
    for(int i = 0; i < len; i++){
      c = esp.read();
      char box = c;
      str += box;
    }
  }
  else{
    str = "NO DATA";
    delay(100);
  }
  return str;
}


/************************************
        WiFi data update
*************************************/
void wifi_update(){
  String str = ble_get_data();
  if(str.indexOf("SSID:") == 0){
    WIFI_SSID = str.substring(5);
    wifi_init();
  }
  else if(str.indexOf("PASSWORD:") == 0){
    WIFI_PASSWORD = str.substring(9);
    wifi_init();
  }
  else if(str.indexOf("IR_HEADER:") == 0){
    IR_HEADER = str.substring(10);
  }
  else if(str.indexOf("IR_NUMBER:") == 0){
    IR_NUMBER = str.substring(10);
  }
//  Serial.print(WIFI_SSID + "//");
//  Serial.println(WIFI_PASSWORD);
//  Serial.println("-------------------------------------------------");
}

