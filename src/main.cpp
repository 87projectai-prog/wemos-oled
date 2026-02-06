#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* ================= OLED ================= */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/* ================= WIFI ================= */
ESP8266WebServer server(80);
const char* AP_NAME = "Leicha Nav";   // <<< NAMA WIFI

/* ================= NAV DATA ================= */
String navDir  = "STRAIGHT"; // LEFT, RIGHT, UTURN, STRAIGHT
int    navDist = 0;
String navRoad = "";

/* ================= BOOT LOGO ================= */
void drawMercedesLogo() {
  display.clearDisplay();

  // Logo star
  display.drawCircle(64, 26, 20, SSD1306_WHITE);
  display.drawLine(64, 26, 64, 6, SSD1306_WHITE);
  display.drawLine(64, 26, 46, 42, SSD1306_WHITE);
  display.drawLine(64, 26, 82, 42, SSD1306_WHITE);

  // Text
  display.setTextSize(1);
  display.setCursor(26, 50);
  display.print("MERCEDES BENZ");

  display.display();
}

/* ================= MAIN MENU ================= */
void drawMainMenu() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("MERCEDES HUD");

  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  display.setCursor(0, 22);
  display.print("NAV READY");

  display.setCursor(0, 34);
  display.print("WiFi: ");
  display.print(AP_NAME);

  display.setCursor(0, 46);
  display.print("WAITING DATA");

  display.display();
}

/* ================= ARROW DRAW ================= */
void drawArrow() {
  if (navDir == "LEFT") {
    display.drawLine(64, 8, 32, 32, SSD1306_WHITE);
    display.drawLine(32, 32, 64, 56, SSD1306_WHITE);
  } 
  else if (navDir == "RIGHT") {
    display.drawLine(64, 8, 96, 32, SSD1306_WHITE);
    display.drawLine(96, 32, 64, 56, SSD1306_WHITE);
  } 
  else if (navDir == "UTURN") {
    display.drawCircle(64, 32, 16, SSD1306_WHITE);
    display.drawLine(64, 16, 64, 6, SSD1306_WHITE);
  } 
  else { // STRAIGHT
    display.drawLine(64, 56, 64, 8, SSD1306_WHITE);
    display.drawLine(64, 8, 58, 16, SSD1306_WHITE);
    display.drawLine(64, 8, 70, 16, SSD1306_WHITE);
  }
}

/* ================= NAV HUD ================= */
void drawNavHUD() {
  display.clearDisplay();

  drawArrow();

  display.setTextSize(1);
  display.setCursor(42, 42);
  display.print(navDist);
  display.print(" m");

  display.setCursor(0, 54);
  display.print(navRoad);

  display.display();
}

/* ================= HTTP HANDLER ================= */
void handleNav() {
  if (server.hasArg("dir"))  navDir  = server.arg("dir");
  if (server.hasArg("dist")) navDist = server.arg("dist").toInt();
  if (server.hasArg("road")) navRoad = server.arg("road");

  drawNavHUD();
  server.send(200, "text/plain", "OK");
}

/* ================= SETUP ================= */
void setup() {
  Serial.begin(115200);
  delay(100);

  // I2C OLED (Wemos D1 Mini)
  Wire.begin(D2, D1);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED FAIL");
    while (true);
  }

  // Boot logo 3 detik
  drawMercedesLogo();
  delay(3000);

  // Paksa WIFI AP (WAJIB)
  WiFi.disconnect(true);
  delay(100);
  WiFi.mode(WIFI_AP);
  delay(100);
  WiFi.softAP(AP_NAME, nullptr, 6);

  Serial.println("AP STARTED");
  Serial.println(WiFi.softAPIP()); // 192.168.4.1

  // Main menu
  drawMainMenu();

  // Web server
  server.on("/nav", handleNav);
  server.begin();
}

/* ================= LOOP ================= */
void loop() {
  server.handleClient();
}
