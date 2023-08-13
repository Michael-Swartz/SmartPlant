#include <Arduino.h>
#include <WiFi.h>
#include <secrets.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

WiFiClient espClient;
PubSubClient client(espClient);

const char* mqtt_broker = "10.0.0.25";
const char* humTopic = "/office/humidity";
const char* tempTopic = "/office/temperature";
const char* voltageTopic = "/office/voltage";
const int mqtt_port = 1883;
const char* ssid = SECRET_SSID;
const char* pass = SECRET_PASS;
const float max_voltage = 4.01; // R1 150 R2 560
const float max_adc_value = 4095.00;

const int analogInPin = 32;
float voltage = 0;

#define DHTPIN 19
#define DHTTYPE DHT22

#define MSG_BUFFER_SIZE	(50)
char temp_msg[MSG_BUFFER_SIZE];
char hum_msg[MSG_BUFFER_SIZE];
char volt_msg[MSG_BUFFER_SIZE];

DHT dht(DHTPIN, DHTTYPE);

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
  Serial.println("Starting up...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());

  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());

  Serial.println();
  delay(5000);
  Serial.println("Connected, IP Address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_broker, mqtt_port);

  pinMode(DHTPIN, INPUT);
  dht.begin();

}

void loop() {

  if (!client.connected()){
    reconnect();
  }

  voltage = ((float(analogRead(analogInPin)) / max_adc_value) * max_voltage);
  Serial.print("Voltage: ");
  Serial.println(voltage);
  Serial.println(analogRead(analogInPin));
  snprintf(volt_msg, MSG_BUFFER_SIZE, "%f", voltage);
  if (client.publish(voltageTopic, volt_msg)) {
    Serial.println("SUCCESS");
  } else {
    Serial.println("FAILURE");
  }

  //Read temperature as Fahrenheit (isFahrenheit = true)
  float f_temp = dht.readTemperature(true);
  Serial.print("Temperature: ");
  snprintf(temp_msg, MSG_BUFFER_SIZE, "%f", f_temp);
  Serial.println(temp_msg);

  if (client.publish(tempTopic, temp_msg)) {
    Serial.println("SUCCESS");
  } else {
    Serial.println("FAILURE");
  }

  // // Read temperature as Fahrenheit (isFahrenheit = true)
  float f_hum = dht.readHumidity();
  Serial.print("Humidity: ");
  snprintf(hum_msg, MSG_BUFFER_SIZE, "%f", f_hum);
  Serial.println(hum_msg);

  if (client.publish(humTopic, hum_msg)) {
    Serial.println("SUCCESS");
  } else {
    Serial.println("FAILURE");
  }

  //Serial.println("Going to sleep now...");
  delay(1000);
  //ESP.deepSleep(60e6);
}