#include <ArduinoJson.h>
#include <SPI.h>
#include <WiFi101.h>
#include <PubSubClient.h>

const char* ssid = "ssid";
const char* password = "ssid_pas";
const char* mqtt_server = "api.mesosfer.com";
const char* deviceid = "deviceid";
const char* devicepass = "devicepass";
const char* channel = "device/[deviceid]/environment"; 
//const char* channel = "device/depokduck/environment"; 
const char* appId = "appId";
const char* devicepass = "schemaId";

WiFiClient wifi;
PubSubClient client(wifi);

char buf[256];
long temperature, humidity;

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loadBuffer(long temperature, long humidity) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["appId"] = appId;
  root["schemaId"] = schemaId;
  root["bucket"] = "Environtment";

  JsonObject& metadata = root.createNestedObject("metadata");
  metadata["humidity"] = String(humidity);
  metadata["temperature"] = String(temperature);

  root.printTo(buf, sizeof(buf));
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "MKR1000Client-";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect broker
    if (client.connect(clientId.c_str(), deviceid, devicepass)) {
      Serial.println("connected");

      temperature = random(10, 30);
      humidity = random(50, 80);
      loadBuffer(temperature, humidity);
      Serial.print("sending message....");
      //client.subscribe("Your topic or channel");
      client.publish(channel, buf);
      Serial.println(buf);
      Serial.println("sent");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
}

