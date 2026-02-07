#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
#include "mercedes_logo.h"

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Input pins
#define PIN_FUEL A0
#define PIN_BATT A0
#define PIN_LEFT_SIGNAL D5
#define PIN_RIGHT_SIGNAL D6
#define PIN_DOOR D7

// HUD variables
int speed = 0;
int rpm = 0;
int fuel = 100;
int battery = 100;
bool leftSignal = false;
bool rightSignal = false;
bool doorOpen = false;
unsigned long lastBlink = 0;
bool blinkState = false;
long odometer = 0;

// Convert voltage to %
int voltageToPercent(float voltage, float minV, float maxV){
  if(voltage<minV) voltage=minV;
  if(voltage>maxV) voltage=maxV;
  return (int)((voltage-minV)*100.0/(maxV-minV));
}

// Booting logo
void bootingLogo(){
  display.clearDisplay();
  display.drawBitmap(48,0, mercedesLogo, 32,32, SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(28,40);
  display.print("Mercedes-Benz Bus");
  display.display();
  delay(3000);
}

void setup(){
  Serial.begin(115200);
  pinMode(PIN_LEFT_SIGNAL, INPUT_PULLUP);
  pinMode(PIN_RIGHT_SIGNAL, INPUT_PULLUP);
  pinMode(PIN_DOOR, INPUT_PULLUP);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  bootingLogo();
}

void loop(){
  unsigned long currentMillis = millis();
  if(currentMillis - lastBlink > 500){
    blinkState = !blinkState;
    lastBlink = currentMillis;
  }

  // Sensor nyata
  float fuelVoltage = analogRead(PIN_FUEL)*(3.3/1023.0);
  fuel = voltageToPercent(fuelVoltage, 0.3, 2.7);

  float battVoltage = analogRead(PIN_BATT)*(3.3/1023.0);
  battery = voltageToPercent(battVoltage, 2.9, 3.3);

  // Lampu sein & pintu
  leftSignal = !digitalRead(PIN_LEFT_SIGNAL) && blinkState;
  rightSignal = !digitalRead(PIN_RIGHT_SIGNAL) && blinkState;
  doorOpen = !digitalRead(PIN_DOOR);

  // Simulasi speed & RPM
  speed = (speed+1)%120;
  rpm = (rpm+50)%4000;
  odometer += speed/100;

  // Clear display
  display.clearDisplay();

  // Logo kecil
  display.drawBitmap(0,0, mercedesLogo,16,16, SSD1306_WHITE);

  // Speedometer melingkar
  int centerX=64, centerY=32, radius=25;
  display.drawCircle(centerX,centerY,radius,SSD1306_WHITE);
  float angle = map(speed,0,120,-90,90);
  float rad = angle*3.14159/180.0;
  int xEnd=centerX+radius*cos(rad);
  int yEnd=centerY+radius*sin(rad);
  display.drawLine(centerX,centerY,xEnd,yEnd,SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(50,0); display.print(speed);

  // RPM melingkar
  int rpmRadius=18;
  float rpmAngle=map(rpm,0,4000,-90,90);
  float rpmRad=rpmAngle*3.14159/180.0;
  int rpmX=centerX+rpmRadius*cos(rpmRad);
  int rpmY=centerY+rpmRadius*sin(rpmRad);
  display.drawLine(centerX,centerY,rpmX,rpmY,SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(centerX-10,centerY+10);
  display.print("RPM");

  // Fuel & Battery bars
  display.drawRect(0,55,64,8,SSD1306_WHITE);
  display.fillRect(0,55,fuel*64/100,8,SSD1306_WHITE);
  display.setCursor(0,46); display.print("FUEL");

  display.drawRect(70,55,58,8,SSD1306_WHITE);
  display.fillRect(70,55,battery*58/100,8,SSD1306_WHITE);
  display.setCursor(70,46); display.print("BATT");

  // Lampu sein
  display.setCursor(0,36); display.print("L:"); if(leftSignal) display.print("<");
  display.setCursor(30,36); display.print("R:"); if(rightSignal) display.print(">");

  // Door
  display.setCursor(70,36); display.print("DOOR:"); display.print(doorOpen?"OPEN":"CLOSE");

  // Odometer
  display.setTextSize(1);
  display.setCursor(50,58);
  display.print("ODO:"); display.print(odometer);

  display.display();
  delay(100);
}
