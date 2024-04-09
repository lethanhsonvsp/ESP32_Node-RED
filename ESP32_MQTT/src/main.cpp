#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "TP-Link_3C7E";
const char *password = "41546840";
const char *mqtt_server = "192.168.0.103";
const int mqtt_port = 2001;
const char *mqtt_id = "esp32";
const char *topic_subscribe = "to-esp32";
const char *topic_publish = "from-esp32";

WiFiClient client;
PubSubClient mqtt_client(client);

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Recived data from: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (size_t i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("------------------------------------------");
}

void setup()
{
  Serial.begin(9600);
  Serial.print("Connecting to Wifi... ");
  Serial.print(ssid);
  Serial.println();
  WiFi.begin(ssid, password);
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

  mqtt_client.setServer(mqtt_server, mqtt_port);
  mqtt_client.setCallback(callback);

  Serial.println("Connecting to mqtt... ");
  while (!mqtt_client.connect(mqtt_id))
  {
    delay(500);
  }
  Serial.println("Connected to mqtt ");
  mqtt_client.subscribe(topic_subscribe);
  mqtt_client.publish(topic_publish, "Hello Server");
}

void loop()
{
  mqtt_client.loop();
}