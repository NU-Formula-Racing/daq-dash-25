/*
  SD card datalogger
 
 This example shows how to log data from two digital sensors and one analog sensor
 to an SD card using the SD library.

In this example you can fiddle with a potentiometer, 
press a button to save the value of the pot to the teensy 4.1 microSD 
and press a different button to display what value was saved.
 	
 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11, pin 7 on Teensy with audio board
 ** MISO - pin 12
 ** CLK - pin 13, pin 14 on Teensy with audio board
 ** CS - pin 4,  pin 10 on Teensy with audio board
 
 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe
then modified again (horrible) by Andrew C. 

 This example code is in the public domain.
 	 
 */

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// Teensy audio board: pin 10
// Teensy 3.5 & 3.6 & 4.1 on-board: BUILTIN_SDCARD
// Wiz820+SD board: pin 4
// Teensy 2.0: pin 0
// Teensy++ 2.0: pin 20
//===============================================================================================================================


const int chipSelect = BUILTIN_SDCARD; 


//create an integer named "pot" This integer will be used for the value of the potentiometer.
int pot; 


//================================================================================================================================
//this part sets things up!

void setup()
{


  //UNCOMMENT THESE TWO LINES FOR TEENSY AUDIO BOARD:
  //SPI.setMOSI(7);  // Audio shield has MOSI on pin 7
  //SPI.setSCK(14);  // Audio shield has SCK on pin 14

  
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect.
  }


  Serial.print("Initializing SD card...");
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

//================================================================================================================================


void loop()
{
  // make a string for assembling the data to log:
  String dataString = "";


//================================================================================================================================
      //if button wired to digital pin 7 is pressed, print "button 7 pressed - data saved!" to serial
        
        if (digitalRead(7) == HIGH) {
        Serial.println("button 7 pressed - data saved!");
        
      //read the potentiometer's value. then use this value as the new value of dataString
          int pot = analogRead(A0);
          dataString = String(pot);


      // open the file named datalog.txt on the sd card
          File dataFile = SD.open("datalog.txt", FILE_WRITE);

          // if the file is available, write the contents of datastring to it
          if (dataFile) {
          dataFile.println(dataString);
          dataFile.close();
          }  
          // if the file isn't open, pop up an error:
          else {
          Serial.println("error opening datalog.txt");
        }   
  }

//================================================================================================================================

//if button wired to digital pin 8 is pressed, print "button 8 pressed - load" to serial


        if (digitalRead(8) == HIGH) {
        Serial.println("button 8 pressed - load");

      //open up datalog2.txt and then print all of its contents  

        File dataFile = SD.open("datalog.txt");
        if(dataFile) {
          Serial.println("datalog:");
          while (dataFile.available()) {
              Serial.write(dataFile.read());
           }
      // close the file:
         dataFile.close();


//Delete the file so it can be created again at the begining of the loop
SD.remove("datalog.txt");
    
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening datalog.txt");
  }
  
}



  delay(150);
  }






