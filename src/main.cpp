#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <secrets.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

const char* mqtt_broker = "10.0.0.157";
const char* topic = "/esp/test";
const int mqtt_port = 1883;
const char* ssid = SECRET_SSID;
const char* pass = SECRET_PASS;
const char* message = "testestestestest";

#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];


void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT Connection....");

    String clientId = "SmartPlant-";
    clientId += String(random(0xffff), HEX);
    Serial.print("Client ID: ");
    Serial.println(clientId);

    if (client.connect(clientId.c_str())){
      Serial.println("Connected");
    } else {
      Serial.print("Failed to connect, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}


void setup() {

  Serial.begin(9600);

  WiFi.begin(ssid,pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  Serial.println("Connected, IP Address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_broker, mqtt_port);

}

void loop() {

  if (!client.connected()){
    reconnect();
  }
  Serial.println("Publishing Message..");
  snprintf(msg, MSG_BUFFER_SIZE, "Hello World");
  if (client.publish(topic, message)) {
    Serial.println("SUCCESS");
  } else {
    Serial.println("FAILURE");
  }
  delay(2000);
}
