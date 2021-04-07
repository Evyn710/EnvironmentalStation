// DHT 11 Temp and Humidity Module
#include <EduIntro.h>
DHT11 dht11(30);

// Photoresistor being used with 10kOhm resistor in series
#define PHOTO_PIN A15

// Sound Sensor
#define SOUND_DIGT_PIN 48
#define SOUND_ANLG_PIN A14

// Motion Sensor
#define PIR_PIN 40

// Real time clock
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;

void setup() {
  Serial.begin(38400);

  // Photoresistor
  pinMode(PHOTO_PIN, INPUT);
  
  // Sound sensor
  pinMode(SOUND_DIGT_PIN, INPUT);
  pinMode(SOUND_ANLG_PIN, INPUT);

  // Motion Sensor
  pinMode(PIR_PIN, INPUT);

  // RTC
  rtc.begin();
  
  if (!rtc.isrunning()) { // makes it so the date and time is only updated once
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }

  delay(1000);
}

void loop() {
  // RTC getting time
  DateTime now = rtc.now();
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
  Serial.println();

  // Print humidity and temperature
  dht11.update();
  float temperature = dht11.readCelsius();
  int humidity = dht11.readHumidity();
  Serial.print("Temperature in C: ");
  Serial.print(temperature);
  Serial.print('\t');
  Serial.println("Humidity: " + humidity);

  // Print light level
  float lightLevel = analogRead(PHOTO_PIN);
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  // Print the noise level
  float sound = analogRead(SOUND_ANLG_PIN);
  Serial.print("Sound Level: ");
  if (digitalRead(SOUND_DIGT_PIN) == HIGH) {
    Serial.println("Too loud");
  } else {
    Serial.println(sound);
  }

  // Print the motion sensor output
  if (digitalRead(PIR_PIN) == HIGH) {
    Serial.println("Motion was detected");
  }
  
  delay(1000);
}
