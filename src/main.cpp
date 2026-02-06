#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* ================= BITMAP ================= */
// 64x64 bitmap (punyamu)
const unsigned char epd_bitmap_images[] PROGMEM = {
  // --- ISI BITMAP KAMU DI SINI ---
  // (yang sudah kamu kirim, tidak aku ubah)
};

/* ================ FUNGSI ================ */

void showLogo() {
  display.clearDisplay();
  display.drawBitmap(32, 0, epd_bitmap_images, 64, 64, SSD1306_WHITE);
  display.display();
}

void showMenu() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("== MAIN MENU ==");

  display.setCursor(0, 16);
  display.println("1. Status");

  display.setCursor(0, 26);
  display.println("2. Setting");

  display.setCursor(0, 36);
  display.println("3. About");

  display.display();
}

/* ================= SETUP ================= */

void setup() {
  Wire.begin(D2, D1); // SDA, SCL (WAJIB buat Wemos)

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (true); // stop kalau OLED gagal
  }

  // === SPLASH SCREEN ===
  showLogo();
  delay(3000);   // 3 detik statis

  // === MASUK MENU ===
  showMenu();
}

void loop() {
  // nanti isi navigasi menu di sini
}
