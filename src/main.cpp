#ifndef NATIVE

#include <Arduino.h>

#include <Dash.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include <iostream>

// Library only supports hardware SPI at this time
// Connect SCLK to UNO Digital #13 (Hardware SPI clock)
// Connect MISO to UNO Digital #12 (Hardware SPI MISO)
// Connect MOSI to UNO Digital #11 (Hardware SPI MOSI)

#define SP28_ANGLE 21
int total_screens = 3;
// Hi!
int screen = 0;
//int re=0;


u_int8_t SP28_input;
float SP28_voltage;

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;

Dash dashboard;

#define LED GPIO_NUM_2
#define BUTTON GPIO_NUM_15
#define green GPIO_NUM_16
#define yellow GPIO_NUM_17
#define red GPIO_NUM_4


  void onRisingEdge() { 
  //re=re+1;
  //screen = (screen + 1) % total_screens;
  screen = screen+1;
  if(screen == 3)
  {
    screen =0;
  }
  }

void setup()
{
  Serial.begin(9600);
  
  pinMode(SP28_ANGLE, INPUT);
  pinMode(LED,OUTPUT);
  pinMode(BUTTON,INPUT);
  pinMode(yellow,OUTPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  // RA8875 Setup
  Serial.println("RA8875 start");

  /* Initialize the display using 'RA8875_480x80', 'RA8875_480x128', 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_800x480))
  {
    Serial.println("RA8875 Not Found!");
    while (1)
      ;
  }

  Serial.println("Found RA8875");

  tft.displayOn(true);
  tft.GPIOX(true);                              // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  // Initialize dashboard
  dashboard.Initialize();

  // With hardware accelleration this is instant
  dashboard.DrawBackground(tft);


  attachInterrupt(digitalPinToInterrupt(BUTTON), onRisingEdge, RISING);

  // writetoSDcard( Serializer.saveData(ourData));

  //

}



void loop()
{
  // put your main code here, to run repeatedly:
  dashboard.GetCAN();
  dashboard.UpdateDisplay(tft);
/*
if(screen == 0)
{
  digitalWrite(LED,1);
  delay(1000);
  digitalWrite(LED,0);
  delay(1000);
  Serial.print(screen);
  Serial.print(re);
  Serial.println("case 0");
  
}
else if(screen == 1)
{
  digitalWrite(LED,1);
  delay(2000);
  digitalWrite(LED,0);
  delay(2000);
  Serial.print(screen);
  Serial.print(re);
  Serial.println("case 1");
}
else if(screen == 2)
{
  digitalWrite(LED,1);
  delay(3000);
  digitalWrite(LED,0);
  delay(3000);
  Serial.print(screen);
  Serial.print(re);
  Serial.println("case 2");
}*/

  switch(screen){
      case 0:
        digitalWrite(red,1);
        digitalWrite(green,0);
        digitalWrite(yellow,0);
        //delay(1000);
        //digitalWrite(LED,0);
        //delay(1000);
        Serial.print("        ");
        Serial.print(screen);
        Serial.println("        case 0");
        break;
      case 1:
        digitalWrite(yellow,1);
        digitalWrite(red,0);
        digitalWrite(green,0);
        //delay(2000);
        //digitalWrite(LED,0);
       // delay(2000);
        Serial.print("        ");
        Serial.print(screen);
        Serial.println("case 1");
        break;
      case 2:
        digitalWrite(green,1);
        digitalWrite(red,0);
        digitalWrite(yellow,0);
        //delay(3000);
        //digitalWrite(LED,0);
        //delay(3000);
        Serial.print("        ");
        Serial.print(screen);
        Serial.println("case 2");
        break;
  }


  // digitalWrite(LED,1);
  /*int var = digitalRead(BUTTON);
  if (var == 1)
  {
    digitalWrite(LED,1);
    delay(1000);
    digitalWrite(LED,0);
  }
  else
  {
    digitalWrite(LED,0);
  }
  */

  // Resistance: 2.6k
  // 180 degrees range of motion: divot side rotating over PF2C1

  // SP28_input = analogRead(SP28_ANGLE);
  // SP28_voltage = (float) SP28_input * 0.00322581f;
  // Serial.println(SP28_input);
  // Serial.println(SP28_voltage);
  // Serial.println("****************************************");
}

#endif