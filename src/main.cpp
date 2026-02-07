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
const char* AP_NAME = "Leicha Nav";   // Nama AP

/* ================= NAV DATA ================= */
String navDir  = "STRAIGHT"; // LEFT, RIGHT, UTURN, STRAIGHT
int    navDist = 0;
String navRoad = "";

/* ================= FUNCTION DECLARATION ================= */
void drawMercedesHUDBoot();
void drawMainMenuAnimated();
void drawArrowAnimated();
void drawArrowStatic();

/* ================= BOOT HUD MERCEDES REAL ================= */
void drawMercedesHUDBoot() {
  display.clearDisplay();
  int cx = 64, cy = 30; // posisi awal logo
  int radius = 18;

  // STEP 1: Layar hitam
  display.clearDisplay();
  display.display();
  delay(500);

  // STEP 2: Logo muncul garis demi garis
  display.drawCircle(cx, cy, radius, SSD1306_WHITE);
  display.display();
  delay(200);

  display.drawLine(cx, cy, cx, cy - radius, SSD1306_WHITE); // atas
  display.display();
  delay(150);

  display.drawLine(cx, cy, cx - radius + 5, cy + radius - 4, SSD1306_WHITE); // kiri bawah
  display.display();
  delay(150);

  display.drawLine(cx, cy, cx + radius - 5, cy + radius - 4, SSD1306_WHITE); // kanan bawah
  display.display();
  delay(150);

  // STEP 3: Teks MERCEDES BENZ fade-in
  String text = "MERCEDES BENZ";
  display.setTextSize(1);
  for (int step = 0; step <= text.length(); step++) {
    display.clearDisplay();
    display.drawCircle(cx, cy, radius, SSD1306_WHITE);
    display.drawLine(cx, cy, cx, cy - radius, SSD1306_WHITE);
    display.drawLine(cx, cy, cx - radius + 5, cy + radius - 4, SSD1306_WHITE);
    display.drawLine(cx, cy, cx + radius - 5, cy + radius - 4, SSD1306_WHITE);

    display.setCursor(22, 52);
    for (int i = 0; i < step; i++) display.print(text[i]);

    display.display();
    delay(120);
  }

  // STEP 4: Logo slide ke atas
  for (int shift = 0; shift <= 12; shift++) {
    display.clearDisplay();
    display.drawCircle(cx, cy - shift, radius, SSD1306_WHITE);
    display.drawLine(cx, cy - shift, cx, cy - radius - shift, SSD1306_WHITE);
    display.drawLine(cx, cy - shift, cx - radius + 5, cy + radius - 4 - shift, SSD1306_WHITE);
    display.drawLine(cx, cy - shift, cx + radius - 5, cy + radius - 4 - shift, SSD1306_WHITE);

    display.setCursor(22, 52 - shift);
    display.print("MERCEDES BENZ");

    display.display();
    delay(80);
  }

  // STEP 5: Main menu HUD muncul
  drawMainMenuAnimated();
}

/* ================= MAIN MENU HUD ANIMASI ================= */
void drawMainMenuAnimated() {
  display.clearDisplay();

  // Header muncul dari atas
  for (int y = -8; y <= 0; y++) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, y);
    display.print("MERCEDES HUD");
    display.drawLine(0, 10 + y, 128, 10 + y, SSD1306_WHITE);
    display.display();
    delay(40);
  }

  // Teks NAV READY, WiFi, WAITING DATA muncul satu per satu
  String lines[3] = {"NAV READY", "WiFi: " + String(AP_NAME), "WAITING DATA"};
  for (int i = 0; i < 3; i++) {
    display.setCursor(0, 22 + i * 12);
    display.print(lines[i]);
    display.display();
    delay(150);
  }
}

/* ================= ARROW HUD ANIMASI ================= */
void drawArrowAnimated() {
  display.clearDisplay();
  int cx = 64, cy = 32;
  int arrowSize = 24;

  // STEP 1: arrow fade-in
  for (int step = 0; step <= 10; step++) {
    display.clearDisplay();

    if (navDir == "LEFT") {
      int x1 = cx, y1 = cy;
      int x2 = cx - arrowSize * step / 10, y2 = cy - arrowSize * step / 10;
      int x3 = cx, y3 = cy + arrowSize * step / 10;
      display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
      display.drawLine(x2, y2, x3, y3, SSD1306_WHITE);
    } 
    else if (navDir == "RIGHT") {
      int x1 = cx, y1 = cy;
      int x2 = cx + arrowSize * step / 10, y2 = cy - arrowSize * step / 10;
      int x3 = cx, y3 = cy + arrowSize * step / 10;
      display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
      display.drawLine(x2, y2, x3, y3, SSD1306_WHITE);
    } 
    else if (navDir == "UTURN") {
      int radius = arrowSize * step / 10;
      display.drawCircle(cx, cy, radius, SSD1306_WHITE);
      display.drawLine(cx, cy - radius, cx, cy - radius - 4, SSD1306_WHITE);
    } 
    else { // STRAIGHT
      display.drawLine(cx, cy + arrowSize/2, cx, cy - arrowSize/2 * step / 10, SSD1306_WHITE);
      display.drawLine(cx, cy - arrowSize/2 * step / 10, cx - 6, cy - arrowSize/2 * step / 10 + 8, SSD1306_WHITE);
      display.drawLine(cx, cy - arrowSize/2 * step / 10, cx + 6, cy - arrowSize/2 * step / 10 + 8, SSD1306_WHITE);
    }

    display.setTextSize(1);
    display.setCursor(42, 50);
    display.print(navDist);
    display.print(" m");

    display.setCursor(0, 58);
    display.print(navRoad);

    display.display();
    delay(50);
  }

  // STEP 2: efek blink arrow
  for (int i = 0; i < 3; i++) {
    display.clearDisplay();
    drawArrowStatic();
    display.setTextSize(1);
    display.setCursor(42, 50);
    display.print(navDist);
    display.print(" m");
    display.setCursor(0, 58);
    display.print(navRoad);
    display.display();
    delay(200);

    display.clearDisplay();
    display.display();
    delay(100);
  }
}

/* ================= ARROW HUD STATIC ================= */
void drawArrowStatic() {
  int cx = 64, cy = 32;
  if (navDir == "LEFT") {
    display.drawLine(cx, cy, cx - 32, cy - 24, SSD1306_WHITE);
    display.drawLine(cx - 32, cy - 24, cx, cy + 24, SSD1306_WHITE);
  } 
  else if (navDir == "RIGHT") {
    display.drawLine(cx, cy, cx + 32, cy - 24, SSD1306_WHITE);
    display.drawLine(cx + 32, cy - 24, cx, cy + 24, SSD1306_WHITE);
  } 
  else if (navDir == "UTURN") {
    display.drawCircle(cx, cy, 16, SSD1306_WHITE);
    display.drawLine(cx, cy - 16, cx, cy - 26, SSD1306_WHITE);
  } 
  else { // STRAIGHT
    display.drawLine(cx, cy + 16, cx, cy - 16, SSD1306_WHITE);
    display.drawLine(cx, cy - 16, cx - 6, cy - 8, SSD1306_WHITE);
    display.drawLine(cx, cy - 16, cx + 6, cy - 8, SSD1306_WHITE);
  }
}

/* ================= SETUP ================= */
void setup() {
  Serial.begin(115200);
  delay(100);
  Wire.begin(D2, D1);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED FAIL");
    while (true);
  }

  drawMercedesHUDBoot();

  // Paksa WIFI AP
  WiFi.disconnect(true);
  delay(100);
  WiFi.mode(WIFI_AP);
  delay(100);
  WiFi.softAP(AP_NAME);
  Serial.println("AP STARTED");
  Serial.println(WiFi.softAPIP());

  // Web server
  server.on("/nav", []() {
    if (server.hasArg("dir"))  navDir  = server.arg("dir");
    if (server.hasArg("dist")) navDist = server.arg("dist").toInt();
    if (server.hasArg("road")) navRoad = server.arg("road");

    drawArrowAnimated();
    server.send(200, "text/plain", "OK");
  });
  server.begin();
}

/* ================= LOOP ================= */
void loop() {
  server.handleClient();
}
