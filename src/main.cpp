#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include "time.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long epochTime;

void initWiFi()
{
  const char *ssid = "WIFI SSID";
  const char *password = "WIFI PASSWORD";
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  display.println("Connecting to WiFi");
  display.display();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    display.print('.');
    display.display();
    delay(1000);
  }
  Serial.print(" Connected! IP: ");
  Serial.print(WiFi.localIP());
  display.clearDisplay();
  display.println("Connected!");
  display.println("IP:");
  display.print(WiFi.localIP());
  display.display();
}

unsigned long getTime()
{
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

void setup()
{
  Serial.begin(115200);
  Serial.println(F("Starting"));

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);

  initWiFi();

  const char *ntpServer = "pool.ntp.org";
  configTime(0, 0, ntpServer);
  delay(2000);
}

void loop()
{
  epochTime = getTime();
  display.clearDisplay();

  display.setCursor(4, 35);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.println(epochTime);

  display.display();
  delay(50);
}
