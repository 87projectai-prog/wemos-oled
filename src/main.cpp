#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
ESP8266WebServer server(80);

// ===== NAV DATA =====
String navDir = "STRAIGHT";
int navDist = 0;
String navRoad = "";

// ===== MERCEDES LOGO =====
void drawMercedesLogo() {
  display.clearDisplay();

  display.drawCircle(64, 26, 20, SSD1306_WHITE);
  display.drawLine(64, 26, 64, 6, SSD1306_WHITE);
  display.drawLine(64, 26, 46, 42, SSD1306_WHITE);
  display.drawLine(64, 26, 82, 42, SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(26, 50);
  display.print("MERCEDES BENZ");

  display.display();
}

// ===== MAIN MENU =====
void drawMainMenu() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("MERCEDES HUD");

  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  display.setCursor(0, 20);
  display.print("NAV READY");

  display.setCursor(0, 32);
  display.print("WiFi: WEMOS_NAV");

  display.setCursor(0, 44);
  display.print("WAITING DATA");

  display.display();
}

// ===== ARROW =====
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
    display.drawLine(64, 16, 64, 8, SSD1306_WHITE);
  }
  else {
    display.drawLine(64, 56, 64, 8, SSD1306_WHITE);
    display.drawLine(64, 8, 58, 16, SSD1306_WHITE);
    display.drawLine(64, 8, 70, 16, SSD1306_WHITE);
  }
}

// ===== NAV HUD =====
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

// ===== HTTP =====
void handleNav() {
  if (server.hasArg("dir")) navDir = server.arg("dir");
  if (server.hasArg("dist")) navDist = server.arg("dist").toInt();
  if (server.hasArg("road")) navRoad = server.arg("road");

  drawNavHUD();
  server.send(200, "text/plain", "OK");
}

// ===== SETUP =====
void setup() {
  Wire.begin(D2, D1);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  drawMercedesLogo();
  delay(3000);

  WiFi.softAP("WEMOS_NAV");

  drawMainMenu();

  server.on("/nav", handleNav);
  server.begin();
}

// ===== LOOP =====
void loop() {
  server.handleClient();
}
