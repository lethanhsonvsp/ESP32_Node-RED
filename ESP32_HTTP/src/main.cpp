#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>

const char *ssid = "TP-Link_3C7E";
const char *password = "41546840";

String serverName = "http://192.168.0.103:1880/esp32";
unsigned long lastTime = 0;
unsigned long timerDelay = 2000;

void httpGET()
{
  WiFiClient client;
  HTTPClient http;

  int temp = random(50, 100);
  String serverPath = serverName + "?temp=" + String(temp);
  http.begin(client, serverPath.c_str());

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

void httpPOST()
{
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName.c_str());

  int humi = random(50, 100);
  String data = "{\"humi\":" + String(humi) + "}";

  int httpResponseCode = http.POST(data);

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

void setup()
{
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if ((millis() - lastTime) > timerDelay)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      httpGET();
      httpPOST();
    }
    else
    {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}