
/************************************
        BLE initialization
*************************************/

void ble_init(){
    esp.setServiceUUID(SERVICE_UUID, sizeof(SERVICE_UUID));
    esp.setCharUUID(CHAR_UUID, sizeof(CHAR_UUID));
    esp.init();
    Serial.println("Made ble UUID");
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



