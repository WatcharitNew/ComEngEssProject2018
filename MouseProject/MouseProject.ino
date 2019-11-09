#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#include <Arduino.h>
#include <String>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

float nowAccelX = 0;
float nowAccelY = 0;

float pastAccelX = 0;
float pastAccelY = 0;


int status = 0;

String ip = "http://192.168.43.149:80/?";

const int LEFT_PIN = 12;
const int RIGHT_PIN = 14;

#define pin A0

void setup() 
{
  Serial.begin(115200);
  Serial.println("ADXL345 Accelerometer Calibration"); 
  Serial.println("");
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
  }
   WiFi.begin("HUAWEI Mate 9","f19fed8ac01a");

   pinMode(LEFT_PIN,INPUT);
   pinMode(RIGHT_PIN,INPUT);
   pinMode(pin, INPUT);
}

void loop()
{
    
    sensors_event_t accelEvent;  
    accel.getEvent(&accelEvent);

//    pastAccelX = accelEvent.acceleration.x;
//    pastAccelY = accelEvent.acceleration.y;
//    Serial.print("X:");Serial.print(pastAccelX);Serial.println();
//    Serial.print("Y:");Serial.print(pastAccelY);Serial.println();
  
    /* Get a new sensor event */ 
    uint16_t value = analogRead (pin);
    double distance = get_IR (value); //Convert the analog voltage to the distance
    Serial.println (value);                 //Print the data to the arduino serial monitor
        Serial.print (distance);
        Serial.println (" cm");
        Serial.println ();
////        delay(500);   
    String warn = (distance <= 6)?("true"):("false");
    String LC = (digitalRead(LEFT_PIN) == HIGH) ? ("true"):("false");
     String RC = (digitalRead(RIGHT_PIN) == HIGH) ? ("true"):("false");
    if(status == 0){
      
      pastAccelX = accelEvent.acceleration.x;
      pastAccelY = accelEvent.acceleration.y;
//      pastAccelZ = accelEvent.acceleration.z;
      status = 1;

      Serial.println(pastAccelX);
      Serial.println(pastAccelY);
    }

    else {
      nowAccelX = accelEvent.acceleration.x;
      nowAccelY = accelEvent.acceleration.y;
//      nowAccelZ = accelEvent.acceleration.z;
//
    Serial.print("pAX: " );Serial.print(pastAccelX);Serial.print(",pAY:");Serial.print(pastAccelY);
    Serial.print("nAX: " );Serial.print(nowAccelX);Serial.print(",nAY:");Serial.print(nowAccelY);

      if(LC == "true")  Serial.println("LeftClick");
      if(RC == "true")  Serial.println("RightClick");
      
      if ((WiFi.status() == WL_CONNECTED)) {
        
        HTTPClient http;

//        Serial.print("[HTTP] begin...\n");
        http.begin(ip + "pX=" + (int)pastAccelX + "&pY=" + (int)pastAccelY
                    + "&nX=" + (int)nowAccelX + "&nY=" + (int)nowAccelY
                    +"&LC=" + LC + "&RC=" + RC + "&Wn=" + warn); //HTTP  

//        Serial.print("[HTTP] GET...\n");
   
        int httpCode = http.GET();
//        Serial.printf("%d",httpCode);  
        if (httpCode > 0) {
   
//          Serial.printf("[HTTP] GET... code: %d\n", httpCode);
          
          if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
//            Serial.println(payload);
          }
        } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        if(LC == "true" || RC == "true"){
          while(digitalRead(LEFT_PIN) == HIGH || digitalRead(RIGHT_PIN) == HIGH){
            true;
          }  
        }
        http.end();
      }
    }
////    delay(10);
        
                
}
double get_IR (uint16_t value) {
        if (value < 16)  value = 16;
        return 2076.0 / (value - 11.0);
}
