/*
 * Project 4
 * Environmental Sensing Station
 * By: Evyn Rissling, Curtis Eck, Brandon Jones
 */

// DHT 11 Temp and Humidity Module
#include <EduIntro.h>
DHT11 dht11(30);

// Photoresistor being used with 10kOhm resistor in series
#define PHOTO_PIN A15

// Sound Sensor
#define SOUND_DIGT_PIN 48
#define SOUND_ANLG_PIN A14

// Real time clock
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;

// Water level sensor
#define WTR_PIN A10

// Bluetooth serial
#include <SoftwareSerial.h>
SoftwareSerial mySerial(51, 50);

void setup() {
  mySerial.begin(38400);

  // Photoresistor
  pinMode(PHOTO_PIN, INPUT);
  
  // Sound sensor
  pinMode(SOUND_DIGT_PIN, INPUT);
  pinMode(SOUND_ANLG_PIN, INPUT);

  // RTC
  rtc.begin();
  
  if (!rtc.isrunning()) { // makes it so the date and time is only updated once
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }

  // Water sensor
  pinMode(WTR_PIN, INPUT);

  
  delay(1000);
}

void loop() {
  // RTC getting time
  DateTime now = rtc.now();
  mySerial.print(now.hour(), DEC);
  mySerial.print(":");
  mySerial.print(now.minute(), DEC);
  mySerial.print(":");
  mySerial.print(now.second(), DEC);
  mySerial.println();

  // Print humidity and temperature
  dht11.update();
  float temperature = dht11.readCelsius();
  int humidity = dht11.readHumidity();
  mySerial.print("Temperature in C: ");
  mySerial.print(temperature);
  mySerial.print("   ");
  mySerial.print("Humidity: ");
  mySerial.print(humidity);
  mySerial.println('%');

  // Print light level
  float lightLevel = analogRead(PHOTO_PIN);
  mySerial.print("Light Level: ");
  mySerial.println(lightLevel);

  // Print the noise level
  int triggered = 0;
  for (int i = 0; i < 100; i++) {
      if(analogRead(SOUND_ANLG_PIN) > 23) {
        triggered = 1;
        break;
      }
  }
  
  mySerial.print("Noise sensor: ");
  if (triggered) {
    mySerial.println("Triggered");
  } else {
    mySerial.println("Not triggered");
  }

  // Print the water sensor output 
  int level = analogRead(WTR_PIN);
  mySerial.print("Water level: ");
  if (level < 320 && level >= 20) {
    mySerial.println("Wet");
  } else if (level >= 320 && level < 350) {
    mySerial.println("Low");
  } else if (level >= 350 && level < 375) {
    mySerial.println("Medium");
  } else if (level >= 375) {
    mySerial.println("High");
  } else {
    mySerial.println("Dry");
  }

  mySerial.println();
  delay(1000);
}
