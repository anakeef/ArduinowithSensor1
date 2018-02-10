/***************************************************************************

 ***************************************************************************/
#include <LiquidCrystal.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define REDLITE 3
#define GREENLITE 5
//#define BLUELITE 10

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); 
LiquidCrystal lcd(9, 8, 7, 6, 4, 2);

float tempC, tempF, humidity, pressure;

long previousMillis = 0;
long interval1 = 5000;
long interval2 = 10000;
long interval3 = 15000;
long interval4 = 20000;

int count = 0;
int brightness = 255;

void setup() {
    Serial.begin(9600);
    Serial.println(F("BME280 test"));
    lcd.begin(16, 2);
    lcd.clear();

    pinMode(REDLITE, OUTPUT);
    pinMode(GREENLITE, OUTPUT);
    setBacklight(255, 255, 0);
   
    lcd.setCursor(0,0);
    lcd.print("Well....");
    lcd.setCursor(0,1);
    lcd.print("   Hello Nora!");
    

    bool status;
    
    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    status = bme.begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    
    Serial.println("-- Default Test --");
    Serial.println();
    delay(5000); // let sensor boot up

}


void loop() { 
    // put your main code here, to run repeatedly:
    unsigned long currentMillis = millis();
    long randNumber = random(1,20);


    if((currentMillis - previousMillis) < interval1)
    {
      if (count == 0) 
      {
        printTemp();
        count = 1;
      }
    }
  
    else if (((currentMillis -previousMillis) > interval2) && ((currentMillis -previousMillis) < interval3))
    {
      if (count == 1) 
      {
        printHumidity();
        count = 2;
      }
      
    }
    else if (((currentMillis - previousMillis) > interval3) && ((currentMillis -previousMillis) < interval4))
    {
      if (count == 2) 
      {
        printPressure();
                
        if (pressure > 29.85){
          setBacklight(0, 255, 0);
        }
        if (pressure < 29.85){
          setBacklight(255, 255, 0);
        }
        if (pressure < 29.60){
          setBacklight(255, 100, 0);
        }
        if (pressure < 29.50){
          setBacklight(255, 0, 0);
        }
        
        count = 0;
      }

      
    }
    
    else if ((currentMillis - previousMillis) > interval4)
    {
      previousMillis = currentMillis;
    }
    
}


void printTemp() {
    lcd.clear();
    tempC =bme.readTemperature();
    tempF = tempC*9/5 +32;
    lcd.setCursor(0,0);
    lcd.print("Temperature ");
    lcd.setCursor(3,1);
    lcd.print(tempF);
    lcd.setCursor(9,1);
    lcd.print("degF");
}

void printHumidity() {
    lcd.clear();
    humidity =bme.readHumidity();
    lcd.setCursor(0,0);
    lcd.print("Humidity ");
    lcd.setCursor(4,1);
    lcd.print(humidity);
    lcd.setCursor(9,1);
    lcd.print("%");
}


void printPressure() {
    lcd.clear();
    pressure =(bme.readPressure() / 3386.39) ;
    lcd.setCursor(0,0);
    lcd.print("Barometric Press ");
    lcd.setCursor(3,1);
    lcd.print(pressure);
    lcd.setCursor(9,1);
    lcd.print("inHg");
}


void setBacklight(uint8_t r, uint8_t g, uint8_t b) {
  // normalize the red LED - its brighter than the rest!
  
  r = map(r, 0, 255, 0, 150);
  g = map(g, 0, 255, 0, 150);
 
  r = map(r, 0, 255, 0, brightness);
  g = map(g, 0, 255, 0, brightness);
 
  // common anode so invert!
  r = map(r, 0, 255, 255, 0);
  g = map(g, 0, 255, 255, 0);

  analogWrite(REDLITE, r);
  analogWrite(GREENLITE, g);
}
