#include <LiquidCrystal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define heart 13                              

const int rs = 15, en = 2, d4 =4 , d5 = 5, d6 = 18, d7 = 19;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


int Buzzer=23;


#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000

float BPM, SpO2;
int count=0;


PulseOximeter pox;
uint32_t tsLastReport = 0;

char ch;

 #ifdef __cplusplus
  extern "C" {
 #endif

  uint8_t temprature_sens_read();

#ifdef __cplusplus
}
#endif

uint8_t temprature_sens_read();



void onBeatDetected()
{
  Serial.println("Beat Detected!");
}



void setup() 
{
  
  pinMode(Buzzer,OUTPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);  //Initialize serial
  Serial2.begin(9600);
   lcd.clear();
  lcd.print("REMOTE PATIENT");
  lcd.setCursor(0, 1);
  lcd.print("MONITORING S/M");
  delay(1000);
  Serial.println("FACE RECOGNITION BASED DATA MONITORING ");
    Serial.print("Initializing pulse oximeter..");

  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");

    pox.setOnBeatDetectedCallback(onBeatDetected);
  }

  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}
void Init_spo2()
{
   Serial.print("Initializing pulse oximeter..");
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");

    pox.setOnBeatDetectedCallback(onBeatDetected);
  }

  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);



}
void loop() 
{
  
  if(Serial2.available()>0)
  {
    ch=Serial2.read();

    Serial.println(ch);

    if(ch=='A')
    {
      while(1)
      {
        int i=0;
        while(i<10000)
        {
            pox.update();
            BPM = pox.getHeartRate();
            SpO2 = pox.getSpO2();
          
            if (millis() - tsLastReport > REPORTING_PERIOD_MS)
            {
          
              Serial.print("HB: ");
              Serial.println(BPM);
          
              Serial.print("SpO2: ");
              Serial.print(SpO2);
              Serial.println("%");
               lcd.clear();
               lcd.print("HB:");
               lcd.print(BPM);
               lcd.setCursor(0, 1);
              lcd.print("SpO2: ");
              lcd.print(SpO2);
          
              lcd.print("%");
             
          
              Serial.println("*********************************");
              Serial.println();
              
          
              tsLastReport = millis();
            }
    //        delay(500);
            i++;
        }
          TEMP_READ();
        }
    }
  }
}
void TEMP_READ()
{
  int adcVal = analogRead(35);
  float milliVolt = adcVal * (5000.0 / 4096);
  float temp35 = milliVolt / 10;
  if(temp35>40)
  temp35=37.0;
  Serial.print("$Temperature: ");
  Serial.print(temp35);   // print the temperature in °C
  Serial.println("°C#");
   lcd.clear();
          lcd.print("TEMP: ");
          lcd.print(temp35);
          lcd.print("C");
  

  delay(1000);
  if((SpO2>10)&&(BPM>10))
  {
// String one = SpO2;
        String two = ",";   
//        String three = BPM;  
        String message = SpO2+two+BPM+two+temp35;
        
        // Convert String to char array
        int str_len = message.length() + 1;
        char textmessage[str_len];
        message.toCharArray(textmessage,str_len);
        Serial.println(textmessage);
        Serial2.println(textmessage);
 Init_spo2();
  }
Init_spo2();
}
