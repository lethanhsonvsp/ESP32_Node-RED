#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <HardwareSerial.h>

WebSocketsClient webSocket;

const char *ssid = "redmi7";
const char *password = "11111111";

const char *websocket_server = "192.168.43.225";
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
      Serial.println("Sending signal to STM32 to turn on LED");
      Serial2.println("1"); // Gửi tín hiệu "1" qua UART tới STM32
    }
    else if (message == "0")
    {
      Serial.println("Sending signal to STM32 to turn off LED");
      Serial2.println("0"); // Gửi tín hiệu "0" qua UART tới STM32
    }
    break;
  }
}

void setup()
{
  Serial.begin(9600);

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

  Serial2.begin(9600, SERIAL_8N1, 16, 17); // baud rate 9600, 8-bit data, no parity, 1 stop bit

  webSocket.begin(websocket_server, websocket_port, url);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void loop()
{
  webSocket.loop();

  if (Serial2.available())
  {
    char receivedChar = Serial2.read();
    if (receivedChar == '0')
    {
      Serial.println("Received '0' from STM32");
      webSocket.sendTXT("LED2 is ON");
    }
    else if (receivedChar == '1')
    {
      Serial.println("Received '1' from STM32");
      webSocket.sendTXT("LED2 is OFF");
    }
  }
}
