#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>

// ------------------ CONFIG WIFI ---------------------
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ------------------ CONFIG MQTT ---------------------
const char* mqtt_server = "143.244.221.196";  
const char* mqtt_topic  = "esp32/temperature";

// ------------------ SENSOR DHT22 ---------------------
const int DHT_PIN = 27;
DHTesp dht;

// ------------------ MQTT CLIENT ---------------------
WiFiClient espClient;
PubSubClient client(espClient);

// ------------------ FUNÇÕES ---------------------

void connectWiFi() {
  Serial.print("Conectando ao WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void connectMQTT() {
  Serial.print("Conectando ao MQTT...");
  while (!client.connected()) {
    if (client.connect("ESP32_DHT22_MONITOR")) {
      Serial.println(" conectado!");
    } else {
      Serial.print(" falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando de novo em 2s");
      delay(2000);
    }
  }
}

// ------------------ SETUP ---------------------

void setup() {
  Serial.begin(115200);

  dht.setup(DHT_PIN, DHTesp::DHT22);

  connectWiFi();

  client.setServer(mqtt_server, 1883);
  connectMQTT();

  Serial.println("Sistema MQTT com DHT22 iniciado!");
}

// ------------------ LOOP ---------------------

void loop() {
  const char* ambiente_id = "sala";
  const char* timestamp = "2024-01-01T12:00:00Z"; // Pode trocar depois por NTP

  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();

  // lê temperatura e umidade
  TempAndHumidity data = dht.getTempAndHumidity();
  float temp = data.temperature;
  float hum  = data.humidity;

  // envia MQTT em formato JSON atualizado
  char payload[256];
  snprintf(payload, sizeof(payload),
           "{ \"ambiente_id\": \"%s\", \"temp\": %.2f, \"umid\": %.2f, \"timestamp\": \"%s\" }",
           ambiente_id, temp, hum, timestamp);

  client.publish(mqtt_topic, payload);

  Serial.print("Payload enviado: ");
  Serial.println(payload);

  delay(1000);
}
