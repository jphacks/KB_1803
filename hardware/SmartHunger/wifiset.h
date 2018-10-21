/************************************
          Wi-Fi config
*************************************/
//String WIFI_SSID = "TP-LINK_1064";
//String WIFI_PASSWORD = "78864603";
//String WIFI_SSID = "~(=^･ω･^)";
//String WIFI_PASSWORD = "nyannyan";
String WIFI_SSID = "500KOBE";
String WIFI_PASSWORD = "@KIITO2018";
#define JST   3600*9


/************************************
          Access to Server
*************************************/
WiFiClient client;
const char* host = "c47cf667.ngrok.io";
unsigned int port = 80;


/************************************
          Access to Server
*************************************/
int YEAR = 0;
int MONTH = 0;
int DAY = 0;
int HOUR = 0;
int MIN = 0;
int SEC = 0;
int NOW = 0;


/************************************
        Wi-Fi initialization
*************************************/
void wifi_init()
{
  //---------------------------
  // try WiFi connect
  //---------------------------
  char ssid[WIFI_SSID.length() + 1];
  char pass[WIFI_PASSWORD.length() + 1];
  WIFI_SSID.toCharArray(ssid, WIFI_SSID.length() + 1);
  WIFI_PASSWORD.toCharArray(pass, WIFI_PASSWORD.length() + 1);
  Serial.print(ssid);
  Serial.print("//");
  Serial.println(pass);
  WiFi.begin(ssid, pass);


  //---------------------------
  // wait WiFi connect
  //---------------------------
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }


  //---------------------------
  // try WiFi connect
  //---------------------------
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Error, W-iFi cannot conncted");
    return 0;
  }

  Serial.println();
  Serial.printf("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("==================================");

  configTime( JST, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
  delay(10000);
}


void get_time() {
  time_t t;
  struct tm *tm;

  t = time(NULL);
  tm = localtime(&t);

  YEAR = tm->tm_year + 1900;
  MONTH = tm->tm_mon + 1;
  DAY = tm->tm_mday;
  HOUR = tm->tm_hour;
  MIN = tm->tm_min;
  SEC = tm->tm_sec;
}


/************************************
           送信するデータの形成
 *************************************/
void send_data()
{
  char payload[100] = {};
  strcat(payload, "temperature=");
  strcat(payload, TEMP_c);
  strcat(payload, "&humidity=");
  strcat(payload, HUMI_c);
  strcat(payload, "&wetness=");
  strcat(payload, WET_c);
  strcat(payload, "&co2=");
  strcat(payload, CO2_c);
  strcat(payload, "&tvoc=");
  strcat(payload, TVOC_c);

  //Serial.println(payload);


  /************************************
            HTTPのGETでURLを叩く
  *************************************/
  static uint32_t time_server;
  time_server = millis();
  while ((!client.connect(host, port)) && (millis() - time_server < 50000)) {
    //Serial.println("false");
    delay(1000);
  }

  if ((millis() - time_server) < 20000)
  {
    //Serial.println("get");
    client.print(String("GET ") + "/api?" + String(payload) + " HTTP/1.1\r\n" +
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

    while (client.available())
    {
      String line = client.readStringUntil('\r');
      //Serial.print(line);
    }
    //Serial.println("");
    client.stop();
  }
  //Serial.println("**********************************************");
}


