/************************************
         Wet sensor
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
    TEMP/HUMI sensor module
*************************************/
SI7021 mySensor2;
char TEMP_c[10];
char HUMI_c[10];



/************************************
  CO2/TVOC sensor module initialize
*************************************/
void sensor_init(){
  mySensor.begin();
}



/************************************
            Wet voltage
*************************************/
void getWet() {
  for (int i = 0; i < 100; i++) {
    WET_val += analogRead(A4);
    delay(10);
  }
  WET_val /= 100;
  memset(WET_c, '\0', sizeof(WET_c));
  sprintf(WET_c, "%d", WET_val);
  Serial.print("wet:");
  Serial.println(WET_c);
  WET_val = 0;
}



/************************************
     get from CO2/VTOC sensor
*************************************/
void getAir() {
  mySensor.readAlgorithmResults();
  memset(CO2_c, '\0', sizeof(CO2_c));
  memset(TVOC_c, '\0', sizeof(TVOC_c));
  sprintf(CO2_c, "%d", (int)mySensor.getCO2());
  sprintf(TVOC_c, "%d", (int)mySensor.getTVOC());
  Serial.print("co2:");
  Serial.println(CO2_c);
  Serial.print("tvoc:");
  Serial.println(TVOC_c);
}




/************************************
     get from TEMP/HUMI sensor
*************************************/
void getEnv() {
  memset(TEMP_c, '\0', sizeof(TEMP_c));
  memset(HUMI_c, '\0', sizeof(HUMI_c));
  sprintf(TEMP_c, "%d", (int)mySensor2.getCelsiusHundredths());
  sprintf(HUMI_c, "%d", (int)mySensor2.getHumidityPercent());
  Serial.print("temp:");
  Serial.println(TEMP_c);
  Serial.print("humi:");
  Serial.println(HUMI_c);
}


