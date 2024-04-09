#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char *ssid = "redmi7";
const char *password = "11111111";

String serverName = "http://192.168.43.225:1880/api/esp32/data-log";
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String getTime()
{
  timeClient.update();

  time_t epochTime = timeClient.getEpochTime();

  String formattedTime = timeClient.getFormattedTime();

  // Get a time structure
  struct tm *ptm = gmtime((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;
  int currentYear = ptm->tm_year + 1900;
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  return currentDate + " " + formattedTime;
}

String getTimeURL()
{
  timeClient.update();

  time_t epochTime = timeClient.getEpochTime();

  String formattedTime = timeClient.getFormattedTime();

  // Get a time structure
  struct tm *ptm = gmtime((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;
  int currentYear = ptm->tm_year + 1900;
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  return currentDate + "%20" + formattedTime;
}

void httpGET()
{
  WiFiClient client;
  HTTPClient http;

  int temp = random(50, 100);
  int humi = random(50, 100);
  String time = getTimeURL();
  String serverPath = serverName + "?Temp=" + String(temp) + "&Humi=" + String(humi) + "&Time=" + time;
  Serial.println(serverPath);
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

  int temp = random(50, 100);
  int humi = random(50, 100);
  String data = "{\"Temp\":" + String(temp) + ",\"Humi\":" + String(humi) + ",\"Time\":\"" + getTime()  + "\"}";

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

  timeClient.setTimeOffset(3600 * 7); // GMT +1 = 3600
  timeClient.begin();
}

void loop()
{
  if ((millis() - lastTime) > timerDelay)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      // httpGET();
      httpPOST();
    }
    else
    {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}