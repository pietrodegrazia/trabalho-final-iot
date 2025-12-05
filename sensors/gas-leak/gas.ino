#include <WiFi.h>
#include <PubSubClient.h>

// ------------------ CONFIG WIFI ---------------------
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ------------------ CONFIG MQTT ---------------------
const char* mqtt_server = "143.244.221.196";  
const char* mqtt_topic  = "esp32/gas-leak";

WiFiClient espClient;
PubSubClient client(espClient);

// ------------------ PINO DO SENSOR ---------------------
const int gasPin = 34;
const int samples = 8;

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
    if (client.connect("ESP32_GAS_MONITOR")) {
      Serial.println(" conectado!");
    } else {
      Serial.print(" falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando de novo em 2s");
      delay(2000);
    }
  }
}

int readGasAvg() {
  long sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(gasPin);
    delay(5);
  }
  return (int)(sum / samples);
}

// ------------------ SETUP ---------------------

void setup() {
  Serial.begin(115200);

  analogSetPinAttenuation(gasPin, ADC_11db);

  connectWiFi();

  client.setServer(mqtt_server, 1883);
  connectMQTT();

  Serial.println("Sistema MQTT iniciado!");
}

// ------------------ LOOP ---------------------

void loop() {
  const char* nomeDoAmbiente = "gas";

  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();

  // lê o valor do gás
  int gasVal = readGasAvg();

  // envia MQTT
  char payload[96];
  snprintf(payload, sizeof(payload),
           "{\"valorMedido\":%d, \"nomeDoAmbiente\": \"%s\"}", gasVal, nomeDoAmbiente);
  client.publish(mqtt_topic, payload);

  Serial.print("Valor enviado via MQTT: ");
  Serial.println(gasVal);

  delay(500);
}
