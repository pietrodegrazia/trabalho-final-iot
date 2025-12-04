📘 Sensor de Luz e Movimento

Monitoramento de luminosidade e detecção de presença com ESP32

🧩
 Hardware

ESP32 – Microcontrolador principal

PIR – Sensor de movimento (detecção de presença)

LDR – Fotoresistor para medir luminosidade

Pinos utilizados:

PIR → GPIO 26

LDR → GPIO 34 (ADC)

⚙️
 Funcionamento

O sistema executa continuamente:

Leitura do PIR para detectar movimento

Leitura do LDR para medir a intensidade luminosa

Geração de um JSON contendo o estado atual

Envio dos dados via MQTT para o servidor (ex: Node-RED)

📡
 Comunicação MQTT
Tópico
iot/movimento/luz

Formato JSON enviado
{
  "ambiente_id": "sala",
  "movimento": 1,
  "luz": 1234,
  "timestamp": "2024-01-01T12:00:00Z"
}

Parâmetros
Campo    Tipo    Descrição
ambiente_id    string    Nome do ambiente monitorado
movimento    int    1 = movimento detectado, 0 = sem movimento
luz    int    Nível de luminosidade (0–4095)
timestamp    string    Data/hora ISO 8601 da leitura
🔗
 Integração com Node-RED

O Node-RED pode:

Receber os dados via MQTT

Identificar ambientes escuros com movimento

Acender luzes automaticamente

Registrar histórico de luminosidade

Enviar alertas ou notificações

Criar dashboards com gráficos e indicadores

🛠️
 Configuração
Bibliotecas Necessárias

WiFi

PubSubClient

Ambas disponíveis pelo gerenciador de bibliotecas da Arduino IDE.

💾
 Código Completo (ESP32 + PIR + LDR + MQTT)
#include <WiFi.h>
#include <PubSubClient.h>

// ------------------ CONFIG WIFI ---------------------
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ------------------ CONFIG MQTT ---------------------
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_topic  = "iot/movimento/luz";

// ------------------ PINOS DOS SENSORES ---------------------
const int PIR_PIN = 26;   // PIR Motion Sensor
const int LDR_PIN = 34;   // LDR (Fotoresistor)

// ------------------ MQTT CLIENT ---------------------
WiFiClient espClient;
PubSubClient client(espClient);

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
    if (client.connect("ESP32_PIR_LDR")) {
      Serial.println(" conectado!");
    } else {
      Serial.print(" falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 2s");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  connectWiFi();
  client.setServer(mqtt_server, 1883);
  connectMQTT();

  Serial.println("Sistema MQTT com PIR + LDR iniciado!");
}

void loop() {
  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();

  int movimento = digitalRead(PIR_PIN);
  int luz = analogRead(LDR_PIN);

  const char* ambiente = "sala";
  const char* timestamp = "2024-01-01T12:00:00Z"; // substitua por NTP futuramente

  char payload[256];
  snprintf(payload, sizeof(payload),
    "{ "ambiente_id": "%s", "movimento": %d, "luz": %d, "timestamp": "%s" }",
    ambiente, movimento, luz, timestamp
  );

  client.publish(mqtt_topic, payload);

  Serial.print("Payload enviado: ");
  Serial.println(payload);

  delay(1000);
}

🔧
 Extensões Futuras

Timestamp automático via NTP

Enviar somente quando houver mudança

Acender lâmpadas com relé

Dashboard completo no Node-RED

Sistema integrado com sensor de temperatura
