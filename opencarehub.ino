#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#include "Wire.h"       
#include "I2Cdev.h"     
#include "MPU6050.h" 
//button
const int buttonPin = 4;

int buttonState = 0;                                                                                                               
// Pins
#define TFT_CS   10
#define TFT_DC    7
#define TFT_RST   8

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
   
#define THRESHOLD 5

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

struct MyData {
  byte X;
  byte Y;
  byte Z;
};

MyData data;

int lastX = -1;
int lastY = -1;
int lastZ = -1;

String event="start";

void showGreen() {
  tft.fillScreen(ST77XX_GREEN);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(6);
  tft.setCursor(120, 90);
  tft.print("OK");
  tft.setTextSize(2);
  tft.setCursor(10,200);
  tft.print("Last event:" + event);
}

void showRed() {
  tft.fillScreen(ST77XX_RED);

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(5);
  tft.setCursor(30, 90);
  tft.print("EMERGENCY");
  tft.setTextSize(2);
  tft.setCursor(50,200);
  tft.print("Event:" + event);
}

void setup() {
  Serial.begin(9600);
  tft.init(240, 320);     // ST7789 2"
  tft.setRotation(1);     // Landscape

  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(3);
  tft.setCursor(20,20);
  tft.println("Open Care Hub");

  tft.drawRect(10, 70, 300, 50, ST77XX_WHITE);

  tft.setCursor(20,85);
  tft.print("System Ready");

  delay(2000);
  pinMode(buttonPin, INPUT_PULLUP);
  
  showGreen();  
  
  Wire.begin();
  mpu.initialize();
  Serial.print("MPU initialize ");
   mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  data.X = map(ax, -17000, 17000, 0, 255 );
  data.Y = map(ay, -17000, 17000, 0, 255); 
  data.Z = map(az, -17000, 17000, 0, 255);  
  delay(500);
  Serial.print("Axis X = ");
  Serial.print(data.X);
  Serial.print("  ");
  Serial.print("Axis Y = ");
  Serial.print(data.Y);
  Serial.print("  ");
  Serial.print("Axis Z  = ");
  Serial.println(data.Z);
  // Αποθήκευση αρχικών τιμών
  lastX = data.X;
  lastY = data.Y;
  lastZ = data.Z;
}

void loop() {
 

  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) { 
        
    event="button pressed";

    showRed();
    
    delay(3000);

    showGreen();
    
  }
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  data.X = map(ax, -17000, 17000, 0, 255 );
  data.Y = map(ay, -17000, 17000, 0, 255); 
  data.Z = map(az, -17000, 17000, 0, 255);  
  delay(500);
  Serial.print("Axis X = ");
  Serial.print(data.X);
  Serial.print("  ");
  Serial.print("Axis Y = ");
  Serial.print(data.Y);
  Serial.print("  ");
  Serial.print("Axis Z  = ");
  Serial.println(data.Z);
  if (abs(data.X - lastX) > THRESHOLD ||
    abs(data.Y - lastY) > THRESHOLD ||
    abs(data.Z - lastZ) > THRESHOLD) { 
    event="motion";
    showRed();

    delay(3000);
    
    showGreen();

    // Αποθήκευση νέων τιμών
    lastX = data.X;
    lastY = data.Y;
    lastZ = data.Z;
  }
  else
  {
    Serial.print("No movement");
  }

} 