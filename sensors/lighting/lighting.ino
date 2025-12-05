#include <WiFi.h>
#include <PubSubClient.h>
#include <cmath>
#include <math.h>

// ------------------ CONFIG WIFI ---------------------
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ------------------ CONFIG MQTT ---------------------
const char* mqtt_server = "143.244.221.196";
const char* mqtt_topic  = "esp32/lighting";

// ------------------ PINOS DOS SENSORES ---------------------
const int PIR_PIN = 26;   // PIR Motion Sensor
const int LDR_PIN = 34;   // DIGITAL output of LDR module (LM393 comparator)

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
    if (client.connect("ESP32_PIR_LDR_DIGITAL")) {
      Serial.println(" conectado!");
    } else {
      Serial.print(" falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 2s");
      delay(2000);
    }
  }
}

// ------------------ SETUP ---------------------

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);   // DIGITAL input now

  connectWiFi();
  client.setServer(mqtt_server, 1883);
  connectMQTT();

  Serial.println("Sistema MQTT com PIR + LDR DIGITAL iniciado!");
}

// ------------------ LOOP ---------------------

void loop() {
  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();

  // Read PIR
  int movimento = digitalRead(PIR_PIN);
  Serial.print("MOVIMENTO: ");
  Serial.println(movimento);

  // Read DIGITAL LDR (LM393)
  // HIGH  = bright
  // LOW   = dark
  int luzDigital = digitalRead(LDR_PIN);

  Serial.print("LDR (digital): ");
  Serial.println(luzDigital);

  // Map digital reading to simple lux estimate:
  int lux = (luzDigital == HIGH ? 0 : 1);
  // You can adjust these numbers as you prefer.

  const char* ambiente = "sala";
  const char* timestamp = "2024-01-01T12:00:00Z";

  char payload[256];
  snprintf(payload, sizeof(payload),
    "{ \"ambiente_id\": \"%s\", \"movimento\": %d, \"luz\": %d, \"timestamp\": \"%s\" }",
    ambiente, movimento, lux, timestamp
  );

  client.publish(mqtt_topic, payload);

  Serial.print("Payload enviado: ");
  Serial.println(payload);

  delay(1000);
}
