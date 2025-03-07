#include <Arduino.h>
#include <pins_arduino.h>

#include <dash.h>
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

//fix later cs/ss pin
Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;

Dash dashboard;

// Define the states of the rotary
enum class RotaryState {
    DRIVER,
    WHEEL,
    STATUS
};

#define CLK_ROTARY 23
#define DT_ROTARY 22
#define SW_ROTARY 21
int currentStateCLK;
int lastStateCLK;


void handleRotaryState(RotaryState state) {
  switch (state) {
      case RotaryState::DRIVER:
          Serial.println("Rotary is on DRIVER.");
          break;
      case RotaryState::WHEEL:
          Serial.println("Rotary is on WHEEL.");
          break;
      case RotaryState::STATUS:
          Serial.println("Rotary is on STATUS.");
          break;
      default:
          Serial.println("more states");
          break;
  }
}

void setup()
{
  Serial.begin(9600);
  
  pinMode(SP28_ANGLE, INPUT);
  pinMode(CLK_ROTARY,INPUT);
  pinMode(DT_ROTARY,INPUT);
  pinMode(SW_ROTARY,INPUT_PULLUP);
  lastStateCLK=digitalRead(CLK_ROTARY);
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
}



void loop()
{
  // put your main code here, to run repeatedly:
  dashboard.GetCAN();
  dashboard.UpdateDisplay(tft);

  // Example usage
  currentStateCLK = digitalRead(CLK_ROTARY);
  if (currentStateCLK!=lastStateCLK&&currentStateCLK==1){
    if(digitalRead(DT_ROTARY)!=currentStateCLK){
      screen=(screen+1)%total_screens;
      handleRotaryState(static_cast<RotaryState>(screen));
    }else{
      screen=(screen-1+total_screens)%total_screens;
      handleRotaryState(static_cast<RotaryState>(screen));
    }
    lastStateCLK=currentStateCLK;
    }
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
}

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
*/

  

  // Resistance: 2.6k
  // 180 degrees range of motion: divot side rotating over PF2C1

  // SP28_input = analogRead(SP28_ANGLE);
  // SP28_voltage = (float) SP28_input * 0.00322581f;
  // Serial.println(SP28_input);
  // Serial.println(SP28_voltage);
  // Serial.println("****************************************");