

#include "DHT.h"
#include "TM1637.h"
#include <Wire.h>
#include "Adafruit_TMP006.h"

Adafruit_TMP006 tmp006;

#define DHTPIN 9
#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);
#define CLK 11//pins definitions for TM1637 and can be changed to other ports       
#define DIO 12
TM1637 tm1637(CLK,DIO);

#define INTERRUPT_INPUT 2
float h;
float t;
float objt ;
float diet ;
float uv ;
int pulse_counter = 0;

void setup() {
  Serial.begin(115200); 
  Serial.println("DHTxx test!");
 
  dht.begin();
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL); 
  tmp006.begin();

  digitalWrite(INTERRUPT_INPUT, HIGH);

  attachInterrupt(INTERRUPT_INPUT - 2,
                  interrupt_handler,
                  RISING);  
}

void loop() {
  int8_t NumTab[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};//0~9,A,b,C,d,E,F
  int8_t ListDisp[4];
   
  h = dht.readHumidity();
  t = dht.readTemperature();
  
  objt = tmp006.readObjTempC();
  diet = tmp006.readDieTempC();
  
  uv = readUV();
  //for(int i=0;i<500;i++) delay(2);
  
  printData();
  pulse_counter = 0;
}

void interrupt_handler()
{
  pulse_counter = pulse_counter + 1;
}

void printData()
{
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");    
  } else {
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C  ");
  }
  {
    Serial.print("Object: "); 
    Serial.print(objt);
    Serial.print(" *C  ");
    Serial.print("Die: "); 
    Serial.print(diet);
    Serial.print(" *C  ");    
    Serial.print(uv);
    Serial.print(" mV ");
    Serial.print(pulse_counter);
    Serial.println(" m/s");
    showLED((int)uv);
  }  
}
float readUV()
{
  int   sensorValue;
  float   uv;
  long  sum=0;
  for(int i=0;i<500;i++)
   {  
      sensorValue=analogRead(A0);
      sum=sensorValue+sum;
      delay(2);
   }   
   sum =sum/500.0;
   uv = sum*4980.0/1023.0;
   return uv;
   //Serial.print("The voltage value:");
   //Serial.print(sum*4980.0/1023.0);
   //Serial.println("V");
   //delay(20);
   //Serial.print("\n");  
}

void showLED(int data)
{
    int ListDisp[4] = {0,0,0,0};
    if (data <= 9999) {
      ListDisp[3] = data / 1000;
      ListDisp[2] = (data % 1000)/100;
      ListDisp[1] = (data % 100)/10;
      ListDisp[0] = (data % 10);
    } else {
    }
    {
    tm1637.display(0,ListDisp[3]);
    tm1637.display(1,ListDisp[2]); 
    tm1637.display(2,ListDisp[1]);
    tm1637.display(3,ListDisp[0]);  
    }
}
