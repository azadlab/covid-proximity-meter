/*
 * COVID PROXIMITY METER
 * This is a program that takes measurements from the ultrasonic sensor through I2C and then displays it on a 7-segment LED as well as shows a distance symbol on 8x8 mstrix LED
 * AZAD LAB (www.youtube.com/azadlab)
 */


#include <TM1637.h>
#include <Wire.h>
#include <LedControl.h>

int CLK = 2;
int DIO = 3;

TM1637 tm(CLK,DIO);


int DIN = 5;
int CS = 4;
int LED_CLK = 6;
LedControl lc=LedControl(DIN, LED_CLK, CS,0);

byte keep_away[8]=   {0x18,0x24,0x24,0x7e,0x24,0x24,0x24,0x24};


void setup() {

  Wire.begin();                
  tm.init();
  tm.set(2);

  Serial.begin(9600);          

  lc.shutdown(0,false);
  lc.setIntensity(0,15);
  lc.clearDisplay(0);  
}

int reading = 0;

void loop() {

  measureDistance();
  displayNumber(reading);  
  printByte(keep_away); //Display symbol on 8x8 Matrix LED
  delay(100);
}

void measureDistance()
{
  Wire.beginTransmission(0xF8 >> 1); //Setting the address
  Wire.write(byte(0x00));      // Start transmition
  Wire.write(byte(0x51));      // Setting the units of measurement
  Wire.endTransmission();      

  delay(70);                   

  Wire.beginTransmission(0xF8 >> 1); 
  Wire.write(byte(0x02));   
  Wire.endTransmission();   
  Wire.requestFrom(0xF8 >> 1, 2);  

  if (2 <= Wire.available()) { 
    reading = Wire.read();  
    reading = reading << 8; 
    reading |= Wire.read(); 
    Serial.println(reading); 
  }
}

void displayNumber(int num){   
    tm.display(3, num % 10);   
    tm.display(2, num / 10 % 10);   
    tm.display(1, num / 100 % 10);   
    tm.display(0, num / 1000 % 10);
}

void printByte(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,8-i-1,character[i]);
  }
}
