#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>

WiFiClient client;

const char *ssid = "redmi7";
const char *password = "11111111";

const char *host = "192.168.43.225";
const int port = 5000;
String message_rev = "";
String message_send = "";

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

  while (!client.connect(host, port))
  {
    delay(500);
  }
  Serial.print("Connected to Server: ");
  Serial.println(host);
}

void loop()
{
  while (client.available())
  {
    char c = (char)client.read();
    message_rev += c;
    delay(1);
  }
  if (message_rev.length() > 0)
  {
    Serial.println(message_rev);
    message_rev = "";
  }

  while (Serial.available())
  {
    char c = (char)Serial.read();
    message_send += c;
    if (c == '\n')
    {
      client.write(message_send.c_str());
      message_send = "";
    }
  }
}