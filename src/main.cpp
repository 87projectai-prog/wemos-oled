#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin untuk baca tegangan battery
#define PIN_BATT A0

int battery = 100;

// Fungsi konversi voltase ke %
int voltageToPercent(float voltage, float minV, float maxV){
  if(voltage < minV) voltage = minV;
  if(voltage > maxV) voltage = maxV;
  return (int)((voltage - minV) * 100.0 / (maxV - minV));
}

// Booting simple
void bootingScreen(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(15,28);
  display.print("BOOTING HUD BUS");
  display.display();
  delay(2000);
}

void setup() {
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println("SSD1306 allocation failed");
    for(;;);
  }
  bootingScreen();
}

void loop() {
  // Baca tegangan battery dari A0
  float volt = analogRead(PIN_BATT) * (3.3 / 1023.0);  // Tegangan masuk A0
  battery = voltageToPercent(volt, 2.9, 3.3);          // Range 2.9V - 3.3V

  display.clearDisplay();

  // Battery bar horizontal
  display.drawRect(10,30,108,10,SSD1306_WHITE);
  display.fillRect(10,30,battery*108/100,10,SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(10,20);
  display.print("BATTERY");

  display.display();
  delay(100);
}
