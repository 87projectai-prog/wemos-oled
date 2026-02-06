#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* ===== DRAW MERCEDES LOGO (VECTOR) ===== */
void drawMercedesLogo(int cx, int cy, int r) {
  // Outer circle
  display.drawCircle(cx, cy, r, SSD1306_WHITE);

  // 3 star lines
  for (int i = 0; i < 3; i++) {
    float angle = i * 120 * DEG_TO_RAD - 90 * DEG_TO_RAD;
    int x = cx + cos(angle) * (r - 2);
    int y = cy + sin(angle) * (r - 2);
    display.drawLine(cx, cy, x, y, SSD1306_WHITE);
  }
}

/* ===== BOOT SCREEN ===== */
void showBootLogo() {
  display.clearDisplay();

  // Logo center
  drawMercedesLogo(64, 24, 18);

  // Brand text
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  const char* brand = "Mercedes-Benz";

  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(brand, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((128 - w) / 2, 46);
  display.print(brand);

  display.display();
  delay(3000);
}

/* ===== HUD MENU ===== */
void showHud() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("SPD 0 km/h");

  display.setCursor(88, 0);
  display.print("READY");

  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  display.setCursor(0, 14);
  display.print("RPM  : 0000");

  display.setCursor(0, 26);
  display.print("TEMP : 27 C");

  display.setCursor(0, 38);
  display.print("VOLT : 12.3 V");

  display.drawLine(0, 50, 127, 50, SSD1306_WHITE);

  display.setCursor(36, 54);
  display.print("SYSTEM OK");

  display.display();
}

void setup() {
  delay(300);
  Wire.begin(D2, D1);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }

  showBootLogo();
  showHud();
}

void loop() {
}
