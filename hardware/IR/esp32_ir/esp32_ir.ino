uint32_t width = 0;
char tx_head[] = {0x40,0x04,0x07,0x20,0x00,0x00,0x00,0x60,0xCB};
//char tx_head[] = {0xE7,0x30,0xD1,0x2E,0x16};
//char tx_head[] = {0xE7,0x30,0xE9,0x16,0x16};
char tx_data[] = {0x40,0x04,0x07,0x20,0x00,0x84,0x2C,0x01,0x22,0x00,0x00,0x60,0x06,0x00,0x00,0x01,0x00,0x60,0xD0,0x00};
//char tx_data[] = {0};
uint8_t i = 0, j = 0;
int start_flag = 1;
uint32_t start_t = 0;

// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0

// use 13 bit precission for LEDC timer
#define LEDC_TIMER_13_BIT  13

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     36700

// fade LED PIN (replace with LED_BUILTIN constant for built-in LED)
#define LED_PIN 21

void ir_tx(char* ir_head, char* ir_data);

void setup() 
{
//  pinMode(A10, INPUT);
//  pinMode(21, OUTPUT);
//  digitalWrite(21, LOW);
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL_0);
  Serial.begin(9600);

  delay(1000);
  Serial.println("Start");
 
}

void loop() 
{
  delay(100);
  ir_tx(tx_head, tx_data);

}

void ir_tx(char* ir_head, char* ir_data)
{
  ledcWrite(LEDC_CHANNEL_0, 50);
  delayMicroseconds(3500);

  ledcWrite(LEDC_CHANNEL_0, 0);
  delayMicroseconds(1500);

  for(int i = 0; i < sizeof(ir_head); i++)
  {
    for(int j = 7; j >= 0; j--)
    {
      if(ir_head[i] & (1<<j))
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

  for(int i = 0; i < sizeof(ir_data); i++)
  {
    for(int j = 7; j >= 0; j--)
    {
      if(ir_data[i] & (1<<j))
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

}
