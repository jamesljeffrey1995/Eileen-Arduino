#include <Wire.h>
#include "DS1307.h"
#include <SPI.h>
#include <SD.h>

File myFile;
DS1307 clock;//define a object of DS1307 class
float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void setup()
{
    Serial.begin(9600);
    clock.begin();
    clock.fillByYMD(2022,06,20);//Jan 19,2013
    clock.fillByHMS(21,49,00);//15:28 30"
    clock.fillDayOfWeek(MON);//Saturday
    clock.setTime();//write time to the RTC chip
}
void loop()
{

    sd();
    delay(10000);
}
    /*Function: Display time on the serial monitor*/

void sd (){
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    clock.getTime();
    myFile.print(clock.hour, DEC);
    myFile.print(":");
    myFile.print(clock.minute, DEC);
    myFile.print(":");
    myFile.print(clock.second, DEC);
    myFile.print("  ");
    myFile.print(clock.month, DEC);
    myFile.print("/");
    myFile.print(clock.dayOfMonth, DEC);
    myFile.print("/");
    myFile.print(clock.year+2000, DEC);
    myFile.print(" ");
    myFile.print(clock.dayOfMonth);
    myFile.print("*");
    switch (clock.dayOfWeek)// Friendly printout the weekday
    {
        case MON:
        myFile.print("MON");
        break;
        case TUE:
        myFile.print("TUE");
        break;
        case WED:
        myFile.print("WED");
        break;
        case THU:
        myFile.print("THU");
        break;
        case FRI:
        myFile.print("FRI");
        break;
        case SAT:
        myFile.print("SAT");
        break;
        case SUN:
        myFile.print("SUN");
        break;
    }
    int analogValue = analogRead(A3);
  // Rescale to potentiometer's voltage (from 0V to 3.3V):
    float voltage = floatMap(analogValue, 0, 1024, 0, 5);
    myFile.print("        Analog: ");
    myFile.print(analogValue);
    myFile.println("");
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");
    
    

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

