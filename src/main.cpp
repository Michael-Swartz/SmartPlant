#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <secrets.h>

const char* ssid = SECRET_SSID;
const char* pass = SECRET_PASS;

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

}

void loop() {}
