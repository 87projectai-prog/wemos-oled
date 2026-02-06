#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/* ================= BITMAP ================= */
const unsigned char epd_bitmap_images[] PROGMEM = {
  // >>> PASTE BITMAP 64x64 KAMU DI SINI <<<
  // mulai dari 0x00, 0x00, ...
};
/* ========================================== */

/* ===== SET CONTRAST (FADE REAL OLED) ===== */
void setContrast(uint8_t val) {
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(val);
}

/* ===== FADE IN ===== */
void fadeInLogo() {
  display.clearDisplay();
  display.drawBitmap(32, 0, epd_bitmap_images, 64, 64, SSD1306_WHITE);
  display.display();

  for (int c = 0; c <= 255; c += 4) {
    setContrast(c);
    delay(10); // smooth
  }
}

/* ===== FADE OUT ===== */
void fadeOutLogo() {
  for (int c = 255; c >= 0; c -= 4) {
    setContrast(c);
    delay(10);
  }
}

void setup() {
  Wire.begin(D2, D1); // SDA, SCL (Wemos D1 Mini)

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (1); // stop kalau OLED tidak terdeteksi
  }

  display.clearDisplay();
  setContrast(0);
  display.display();

  fadeInLogo();
  delay(1200);
  fadeOutLogo();

  display.clearDisplay();
  setContrast(255);
  display.display();
}

void loop() {
  // kosong (boot animation only)
}
