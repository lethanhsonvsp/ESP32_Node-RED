#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

#define LED_PIN 2

WebSocketsClient webSocket;

const char *ssid = "LeNgocDan";
const char *password = "20242025";
const char *websocket_server = "192.168.1.24";
const int websocket_port = 1880;
const char *url = "/esp32";

unsigned long last_time = 0;

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{

  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.printf("[WSc] Disconnected!\n");
    break;
  case WStype_CONNECTED:
  {
    Serial.printf("[WSc] Connected to url: %s\n", payload);
    webSocket.sendTXT("Connected");
  }
  break;
  case WStype_TEXT:
    String message = "";
    for (size_t i = 0; i < length; i++)
    {
      message += (char)payload[i];
    }

    if (message == "1")
    {
      digitalWrite(LED_PIN, HIGH);
    }

    if (message == "0")
    {
      digitalWrite(LED_PIN, LOW);
    }

    break;
  }
}

void sendToServer()
{
  if (millis() - last_time > 1000)
  {
    last_time = millis();
    if (webSocket.isConnected())
    {
      int temp = random(50, 100);
      int humi = random(80, 100);
      String payload = "{\"temp\":";
      payload += temp;
      payload += ",\"humi\":";
      payload += humi;
      payload += "}";
      webSocket.sendTXT(payload.c_str());
    }
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  Serial.print("Connecting to Wifi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to Wifi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // server address, port and URL
  webSocket.begin(websocket_server, websocket_port, url);

  // event handler
  webSocket.onEvent(webSocketEvent);

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);
}

void loop()
{
  webSocket.loop();
  sendToServer();
}