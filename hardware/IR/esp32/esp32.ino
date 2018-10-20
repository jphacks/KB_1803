uint32_t width = 0;
char nbyte[8] = {};
int  rx_data[30] = {};
uint8_t i = 0, j = 0;
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
  // put your main code here, to run repeatedly:
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
//  Serial.println(width);
//  Serial.println("Get start cord");
  while(1)
  {

    while(digitalRead(RX_IR) == 0);
    start_t = micros();
    delayMicroseconds(100);
    while(digitalRead(RX_IR) && ((micros() - start_t) < 5600));
//    while(digitalRead(RX_IR) );
    width = micros() - start_t;
//    Serial.println(width);

    if(width > 5500)
    {
//      Serial.println("End cord");
//      Serial.println(width);
      break;
    }
    else if(width > (430 * 2))
    {
      nbyte[i] = 0b1;
    }
    else
    {
      nbyte[i] = 0b0;
    }
    
    if(i == 7)
    {
      rx_data[j] = (nbyte[0]<<7)|(nbyte[1]<<6)|(nbyte[2]<<5)|(nbyte[3]<<4)|(nbyte[4]<<3)|(nbyte[5]<<2)|(nbyte[6]<<1)|nbyte[7];
      j++;
      i = 0;
    }
    else
      i++;
    
  }
//  Serial.print("length = ");
//  Serial.println(j +  1);
  long sum = 0;
  for(int k = 0; k < j; k++)
  {
    if(rx_data[k] < 0x10)
    {
      Serial.print(0,HEX);
      Serial.print(rx_data[k],HEX);
    }
    else
      Serial.print(rx_data[k],HEX);
    Serial.print(",");
    sum += rx_data[k];
    rx_data[k] = 0;
  }
  Serial.print((sum & 0x0000ff),HEX);
  Serial.println("");
//  nbyte[0] = 0;
//  nbyte[1] = 0;
//  nbyte[2] = 0;
//  nbyte[3] = 0;
//  nbyte[4] = 0;
//  nbyte[5] = 0;
//  nbyte[6] = 0;
//  nbyte[7] = 0;
  i = 0;
  j = 0;
  start_flag = 1;

}
