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

#define SHOWDOC_TIME_INTERVAL 1000 // 間隔1秒顯示一個點
#define REPORT_TIME_INTERVAL 30000 // 間隔30秒回報一次
unsigned long previousShowDotMillis = 0;
unsigned long previousUpdateMillis = 0;

// UDP Search Support
WiFiUDP udpSearch;
// UDP Data Buffer
#define UDP_PACKET_SIZE UDP_TX_PACKET_MAX_SIZE
char udpBuffer[UDP_PACKET_SIZE];

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

  // Prepare DHT Sensor
  dht.begin();

  // Prepare UDP search
  udpSearch.begin(8181);
  Serial.print("Start UDP Search support at port: " + String(udpSearch.localPort()));
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long nowMillis = millis();
  // Check if should show dot
  if (previousShowDotMillis == 0 || (nowMillis-previousShowDotMillis > SHOWDOC_TIME_INTERVAL)) {
    previousShowDotMillis = nowMillis;
    Serial.print(".");  
  }
  // Check if should report to server
  if (previousShowDotMillis == 0 || (nowMillis-previousUpdateMillis > REPORT_TIME_INTERVAL)) {
    previousUpdateMillis = nowMillis;
    Serial.print("Start to report to server");
    readFromDHTServer();
  }

  handelUDPSeatchJob();
  
  // Sleep for a while
  delay(10);
}

void readFromDHTServer() {
  float newTemperature = dht.readTemperature();  
  float newHumidity = dht.readHumidity();
  if (isnan(newTemperature) || isnan(newHumidity)) {
    Serial.println("Fail to read from DHT server.");
    return ;
  } else {
    // OK to read Temperature and Humidity, will report to server
    lastTemperature = newTemperature;
    lastHumidity = newHumidity;
    Serial.println("Temperature: " + String(lastTemperature) + "\tHumidity:" + String(lastHumidity));
  }
}

void handelUDPSeatchJob() {
  int packetSize = udpSearch.parsePacket();
  if (packetSize == 0) {
    // No incoming data, and return 
    return ;  
  }
  // Handle incoming data
  Serial.println("\nUDPSearch Receive packet, length=" + String(packetSize));

  udpSearch.read(udpBuffer, UDP_PACKET_SIZE);
  udpBuffer[packetSize] = 0; // EOL
  Serial.println("DUPSearch Receive Content: " + String(udpBuffer));

  //  Reply to remote
  IPAddress remoteIP = udpSearch.remoteIP();
  Serial.print("UDPSearch: RemoteIP: ");
  Serial.print(remoteIP);
  Serial.println(" Remote Port: " + String(udpSearch.remotePort()));

  //Prepare reply data
  String reply = "{\"Temperature\":\"" + String(lastTemperature) + "\",\"Humidity\": \"" + String(lastHumidity) + "\", \"SansorName\": \"" + String(MAC_char) + "\""; 
  reply.toCharArray(udpBuffer, UDP_PACKET_SIZE);
  udpSearch.beginPacket(remoteIP, udpSearch.remotePort());
  udpSearch.write(udpBuffer);
  udpSearch.endPacket();
}
