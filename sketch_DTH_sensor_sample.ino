// 引入函式庫
#include <ESP8266WiFi.h>
#include <WiFiudp.h>
#include <WiFiClient.h>
#include "DHT.h"

// 變數宣告
char ssid[] = "AP NAME";     //  Wifi AP SSID (name)
char password[] = "PASSWORD";       //  Wifi AP password
uint8_t MAC_array[6];
char MAC_char[18];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Prepare WiFi connection
  Serial.print("Trying to connect to " + String(ssid));
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWiFi AP Connectced.");
}

void loop() {
  // put your main code here, to run repeatedly:

}
