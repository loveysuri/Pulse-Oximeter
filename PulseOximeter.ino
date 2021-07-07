

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
 
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "OakOLED.h"
 
#define REPORTING_PERIOD_MS 1000
OakOLED oled;
 
char auth[] = "7uBUOQO5UKM-0hUGg3GtWWz2RdJoWx1g";             
char ssid[] = "HUAWEI-Kc6M";        //WiFi SSID
char pass[] = "Connect@123";        //WiFi Password
 

PulseOximeter pox;
 
float BPM, SpO2;
uint32_t tsLastReport = 0;
 

 
void onBeatDetected()
{
    Serial.println("Beat Detected!");
    
    
}
 
void setup()
{
    
    Serial.begin(115200);
    
    oled.begin();
     oled.clearDisplay();
    oled.setTextSize(2);
    oled.setTextColor(1);
    oled.setCursor(0,0);
    oled.println("\n PULSE\n OXIMETER");
    Serial.println("\n PULSE\n OXIMETER");
    oled.display();
    delay(2000);
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setTextColor(1);
    oled.setCursor(0,0);
    oled.println("BY :\nLOVE KUMAR\nR.no.:3930");
    Serial.println("BY :\nLOVE KUMAR\nR.no.:3930");
    oled.display();

    delay(3000);
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setTextColor(1);
    oled.setCursor(0,30);
    oled.println("STARTING..");
    Serial.println("STARTING..");
    
    oled.display();
    
    pinMode(16, OUTPUT);
    Blynk.begin(auth, ssid, pass);
 
    Serial.print("Initializing Pulse Oximeter..");
 
    if (!pox.begin())
    {
         Serial.println("FAILED");
         oled.clearDisplay();
         oled.setTextSize(2);
         oled.setTextColor(1);
         oled.setCursor(0, 0);
         oled.println("FAILED");
         oled.display();
         for(;;);
    }
    else
    {
         oled.clearDisplay();
         oled.setTextSize(2);
         oled.setTextColor(1);
         oled.setCursor(0, 0);
         oled.println("SUCCESS");
         oled.display();
         Serial.println("SUCCESS");
         pox.setOnBeatDetectedCallback(onBeatDetected);
    }
 
    
 
}
 
void loop()
{
    pox.update();
    Blynk.run();
 
    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
    if (BPM >100 || SpO2>100){
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setTextColor(1);
    oled.setCursor(0,30);
 
    oled.println(" -ERROR-");
    Serial.println(" -ERROR-");
    oled.display();
      }
    else if(BPM==0 && SpO2==0){
    Blynk.virtualWrite(V7, BPM);
    Blynk.virtualWrite(V8, SpO2);
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setTextColor(1);
    oled.setCursor(0,5);
    oled.println("   PLACE\n   YOUR\n   FINGER");
    Serial.println("   PLACE   YOUR   FINGER");
    oled.display();
    
    
      }  
    else{  
    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        Serial.print("Heart rate:");
        Serial.print(BPM);
        Serial.print(" SpO2:");
        Serial.print(SpO2);
        Serial.println(" %");
 
        Blynk.virtualWrite(V7, BPM);
        Blynk.virtualWrite(V8, SpO2);
        
        oled.clearDisplay();
        oled.setTextSize(2);
        oled.setTextColor(1);
        oled.setCursor(0,16);
        oled.println(pox.getHeartRate());
 
        oled.setTextSize(1);
        oled.setTextColor(1);
        oled.setCursor(0, 0);
        oled.println("Pulse Rate :");
 
        oled.setTextSize(1);
        oled.setTextColor(1);
        oled.setCursor(0, 35);
        oled.println("Spo2 % :");
 
        oled.setTextSize(2);
        oled.setTextColor(1);
        oled.setCursor(0,50);
        oled.println(pox.getSpO2());
        oled.display();
 
        tsLastReport = millis();
    }
    }
}
