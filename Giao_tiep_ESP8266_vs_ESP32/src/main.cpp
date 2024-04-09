#include <Arduino.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

WiFiClient client;
PubSubClient mqtt_client(client);

const char *ssid = "TP-Link_3C7E";
const char *password = "41546840";
const char *server_mqtt = "192.168.0.103";
const int port_mqtt = 2001;

const char *mqtt_id = "esp8266";
const char *topic_subscribe = "esp32";
const char *topic_publish = "esp8266";
String message_send = "";

void callback(char *topic, byte *payload, unsigned int length)
{
  String message = "";
  Serial.print("Recived from: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (size_t i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }
  Serial.print(message);
  Serial.println("----------------------------------------");
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Connecting to Wifi...");
  WiFi.begin(ssid, password);
  WiFi.reconnect();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to Wifi ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());
  Serial.println();

  delay(1000);

  mqtt_client.setServer(server_mqtt, port_mqtt);
  mqtt_client.setCallback(callback);

  Serial.println("Connecting to mqtt ");
  while (!mqtt_client.connect(mqtt_id))
  {
    delay(500);
  }
  Serial.println("Connected to mqtt ");
  mqtt_client.subscribe(topic_subscribe);
}

void recivedMessageFromUart()
{
  if (Serial.available())
  {
    char c = (char)Serial.read();
    message_send += c;
    if (c == '\n')
    {
      if (mqtt_client.connected())
      {
        mqtt_client.publish(topic_publish, message_send.c_str());
        message_send = "";
      }
    }
  }
}

void loop()
{
  mqtt_client.loop();
  recivedMessageFromUart();
}