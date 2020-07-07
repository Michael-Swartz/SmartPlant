#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

#define DHTPIN 10
#define DHTTYPE DHT22
float hum;
float temp;

DHT dht(DHTPIN, DHTTYPE);

ESP8266WebServer server(80);

const char* ssid = "ponyfarm_IoT";
const char* password = "mustangsallys";

void start_water_plant(){
  //String water_status = "{\"Watering_Status\": RUNNING }";
  //Serial.println("Starting Pump");
  digitalWrite(14, HIGH);
  server.send(303); 
}

void water_plant(){
  //String water_status = "{\"Watering_Status\": RUNNING }";
  //Serial.println("Starting Pump");
  digitalWrite(14, HIGH);
  delay(1000);
  digitalWrite(14, LOW);
  server.send(303); 
}


void stop_water_plant(){
  //String water_status = "{\"Watering_Status\": STOPPED }";
  //Serial.println("Starting Pump");
  digitalWrite(14, LOW);
  server.send(303); 
}


void water_plant_time() {
  String postBody = server.arg("plain");
  Serial.println(postBody);

  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, postBody);
  if (error) {
    Serial.println("There was an error parsing the JSON");
    Serial.println(error.c_str());
    String msg = error.c_str();
    server.send(400, "text/json","{\"Watering_Status\": ERROR1 }"); 
  } else {
    JsonObject post_content = doc.as<JsonObject>();
    
    Serial.print("HTTP Method: ");
    Serial.println(server.method());

    if (server.method() == HTTP_POST){
      if (post_content.containsKey("time")) {
        int wtime = post_content["time"];
        double secs = wtime / 1000;
        Serial.println("Running pump for " + String(wtime) + " seconds.");
        digitalWrite(14, HIGH);
        delay(wtime);
        digitalWrite(14, LOW);
        server.send(201, "text/json", "{\"Watering_Status\": SUCCESS }");
      } else {
        Serial.println("Error with Post");
        server.send(400, "text/json","{\"Watering_Status\": ERROR2 }");
      }
    }
  } 
}

String get_moisture() {
  int moist = analogRead(A0);
  Serial.println("Soil Moisture: " + String(moist));
  String final_string = "{\"moisture\": " + String(moist) + "}";
  return final_string;
};

String get_weather() {
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  Serial.println("Temp: " + String(temp));
  Serial.println("Hum: " + String(hum));
  String final_string_2 = "{\"temperature\": " + String(temp) + ", " + "\"humidity\": " + String(hum) + "}";
  return final_string_2;
};

void send_soil_moisture() {
  server.send(200, "text/json", get_moisture()); 
};
void send_weather() {
  server.send(200, "text/json", get_weather()); 
};

void handleNotFound(){
  Serial.println("Bunk Page Visited");
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
//
//void restServerRouting() {
//  server.on("/soil_moisture", HTTP_GET, send_soil_moisture);
//  server.on("/water_plant", HTTP_GET, send_watering_status);
//};


void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  pinMode(14, OUTPUT);
  dht.begin();
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Waiting to connect...");
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Server listening");

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
 
  //restServerRouting();
  server.on("/soil_moisture", HTTP_GET, send_soil_moisture);
  server.on("/weather", HTTP_GET, send_weather);
  server.on("/start_water_plant", HTTP_GET, start_water_plant);
  server.on("/stop_water_plant", HTTP_GET, stop_water_plant);
  server.on("/water_plant", HTTP_GET, water_plant);
  server.on("/water_plant_time", HTTP_POST, water_plant_time);
  server.onNotFound(handleNotFound); 
  server.begin();
}

void loop() {
  server.handleClient();
}
