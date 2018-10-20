uint32_t width = 0;
int start_flag = 1;
uint32_t start_t = 0;

#define RX_IR A0


void setup() 
{
  pinMode(RX_IR, INPUT);
  Serial.begin(9600);
}

void loop() 
{
  Serial.println("Start");
  while(start_flag)
  {
    while(digitalRead(RX_IR));
    start_t = micros();
    delayMicroseconds(100);
    while(digitalRead(RX_IR) == 0);
    width = micros() - start_t;
//    Serial.println(width);

    if(width > 3200)
    {
      
      start_t = micros();
//      delayMicroseconds(100);
      while(digitalRead(RX_IR));
      width = micros() - start_t;

      if(width > ((430 * 3) + 50))
        start_flag = 0;
    }
    
  }
}
