

/************************************
        Wi-Fi initialization
*************************************/
void wifi_init()
{
  Serial.println("Reset:");
  char ssid[WIFI_SSID.length() + 1];
  char pass[WIFI_PASSWORD.length() + 1];
  WIFI_SSID.toCharArray(ssid, WIFI_SSID.length()+1);
  WIFI_PASSWORD.toCharArray(pass, WIFI_PASSWORD.length()+1);
  Serial.print(ssid);
  Serial.print("//");
  Serial.println(pass);
  WiFi.begin(ssid, pass);
  
//  for(int count= 0; WiFi.status() != WL_CONNECTED && count<100; count++) {
//    Serial.print('.');
//    delay(500);
//  }

  while(WiFi.status() != WL_CONNECTED){
    Serial.print('.');
    delay(500);
  }
  
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Error, W-iFi cannot conncted");
    return 0;
  }
  
  Serial.println();
  Serial.printf("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  configTime( JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
  delay(10000);
}

now_time sendNTPpacket()
{
  time_t t;
  struct tm *tm;

  t = time(NULL);
  tm = localtime(&t);
//  Serial.println(" %04d/%02d/%02d %02d:%02d:%02d\n",
//        tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
//        tm->tm_hour, tm->tm_min, tm->tm_sec);

  now_time N1;
  N1.now_year = tm->tm_year + 1900;
  N1.now_month = tm->tm_mon+1;
  N1.now_day = tm->tm_mday;
  N1.now_hour = tm->tm_hour;
  N1.now_min = tm->tm_min;
  N1.now_sec = tm->tm_sec;

  return N1;
}

void IR()
{
}

void make_data()
{
  char payload[100]={};
               
/************************************
          送信するデータの形成
*************************************/
//  memset(Temp_c, '\0', sizeof(Temp_c)); 
//  memset(Humi_c, '\0', sizeof(Humi_c)); 
//  memset(Light_c, '\0', sizeof(Light_c));
//
//  dtostrf((float)smeHumidity.readTemperature() - 4.0 , 5, 2, Temp_c);
//  if((float)smeHumidity.readHumidity() < 100) dtostrf((float)smeHumidity.readHumidity(), 5, 2, Humi_c);
//  else dtostrf(100.00, 5, 2, Humi_c);
//
//  OPT3001 result = light.readResult();
//  sprintf(Light_c, "%d", (int)result.lux);
//  for (j = 0; j < ii; j++)
//  {
//    sum += human[j];
//    human[j] = 0;
//  }
//  float w = (float)sum / (float)ii;
//  if(w > 0.001)
//    HUMAN_c = "1";
//  else
//    HUMAN_c = "0";
//    
//  sum = 0;
//  ii = 0;
//  
//  strcat(payload, ",");
//  strcat(payload, Temp_c);
//  strcat(payload, ",");
//  strcat(payload, Humi_c);
//  strcat(payload, ",");
//  strcat(payload, Light_c);
//  strcat(payload, ",");
//  strcat(payload, HUMAN_c);
//  strcat(payload, ",");
//  strcat(payload, CO2_c);
//
//  float temp = (float)smeHumidity.readTemperature() - 3.0;
//

  
/************************************
          HTTPのGETでURLを叩く
*************************************/
  static uint32_t time_server;
  time_server = millis();
  while((!client.connect(host, port)) && (millis() - time_server < 50000)) delay(1000);

  if((millis() - time_server) < 20000) 
  {
    client.print(String("GET ") + "api/post/" + String(payload) + " HTTP/1.1\r\n" +
                        "Host: " + host + "\r\n" + 
                        "Connection: close\r\n\r\n");
    int timeout = millis() + 5000;
    
    while (client.available() == 0) 
    {
      if (timeout - millis() < 0) 
      {
        client.stop();
        return;
      }
    }
    
    while(client.available())
    {
      String line = client.readStringUntil('\r');
    }
    client.stop();
  }
}
