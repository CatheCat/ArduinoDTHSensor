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

// DHT Sensor Support
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE); // 定義 dnt 物件
float lastTemperature; // 脧新一次的溫度
float lastHumidity; // 最新一次的濕度
String lastUpdateTime; // 最新一次的更新時間

#define SHOWDOC_TIME_INTERVAL = 1000; // 間隔1秒顯示一個點
#define REPORT_TIME_INTERVAL = 30000; // 間隔30秒回報一次
unsigned long previousShowDotMillis = 0;
unsigned long previousUpdateMillis = 0;

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

  // Acquire important information
  WiFi.macAddress(MAC_array);
  for(int i=0; i < sizeof(MAC_array); i++) {
    sprintf(MAC_char, "%s%02x", MAC_char, MAC_array[i]);  
  }
  Serial.println("WiFi Mac Address:" + String(MAC_char));
  Serial.print("IP Address:"); 
  Serial.println(WiFi.localIP());
  Serial.print("Gateway:"); 
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS:"); 
  Serial.println(WiFi.dnsIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
