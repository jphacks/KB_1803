/************************************
         Analyzed IR code
*************************************/
String IR_HEADER = "0x40,0x04,0x07,0x20,0x00,0x00,0x00,0x60";
String IR_NUMBER = "x40,0x04,0x07,0x20,0x00,0x84,0x3C,0x01,0xF5,0x00,0x00,0x60,0x06,0x00,0x40,0x01,0x00,0x68,0x09";


/************************************
         Analyzed IR code
*************************************/

uint32_t width = 0;
char tx_head[] = {0x40,0x04,0x07,0x20,0x00,0x00,0x00,0x60};
char tx_data[] = {0x40,0x04,0x07,0x20,0x00,0x84,0x3C,0x01,0xF5,0x00,0x00,0x60,0x06,0x00,0x40,0x01,0x00,0x68,0x09};;
uint8_t i = 0, j = 0;
int start_flag = 1;
uint32_t start_t = 0;
int head_len = sizeof(tx_head);
int data_len = sizeof(tx_data);

// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0

// use 13 bit precission for LEDC timer
#define LEDC_TIMER_13_BIT  13

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     36700

// fade LED PIN (replace with LED_BUILTIN constant for built-in LED)
#define LED_PIN 33


/************************************
          IR initialize
*************************************/
void IR_init()
{
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL_0);
}


/************************************
          IR data send
*************************************/
void IR_send()
{
  ledcWrite(LEDC_CHANNEL_0, 50);
  delayMicroseconds(3500);

  ledcWrite(LEDC_CHANNEL_0, 0);
  delayMicroseconds(1500);

  for(int i = 0; i < head_len; i++)
  {
    for(int j = 7; j >= 0; j--)
    {
      if(tx_head[i] & (1<<j))
      {
        ledcWrite(LEDC_CHANNEL_0, 50);
        delayMicroseconds(430);
        ledcWrite(LEDC_CHANNEL_0, 0);
        delayMicroseconds(430 * 3);
      }
      else
      {
        ledcWrite(LEDC_CHANNEL_0, 50);
        delayMicroseconds(430);
        ledcWrite(LEDC_CHANNEL_0, 0);
        delayMicroseconds(430 * 1);
      }
    }
  }

  ledcWrite(LEDC_CHANNEL_0, 50);
  delayMicroseconds(430);

  ledcWrite(LEDC_CHANNEL_0, 0);
  delayMicroseconds(9960);
  
  ledcWrite(LEDC_CHANNEL_0, 50);
  delayMicroseconds(3500);

  ledcWrite(LEDC_CHANNEL_0, 0);
  delayMicroseconds(1500);

  for(int i = 0; i < data_len; i++)
  {
    for(int j = 7; j >= 0; j--)
    {
      if(tx_data[i] & (1<<j))
      {
        ledcWrite(LEDC_CHANNEL_0, 50);
        delayMicroseconds(430);
        ledcWrite(LEDC_CHANNEL_0, 0);
        delayMicroseconds(430 * 3);
      }
      else
      {
        ledcWrite(LEDC_CHANNEL_0, 50);
        delayMicroseconds(430);
        ledcWrite(LEDC_CHANNEL_0, 0);
        delayMicroseconds(430 * 1);
      }
    }
  }
  ledcWrite(LEDC_CHANNEL_0, 50);
  delayMicroseconds(430);

  ledcWrite(LEDC_CHANNEL_0, 0);
  delayMicroseconds(30 * 430);

  delay(100);
}






