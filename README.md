# Sistema de Monitoramento IoT para Idosos

Sistema de monitoramento inteligente para ambientes residenciais focado na segurança e conforto de idosos, utilizando ESP32 e sensores diversos.

## Funcionalidades

### Controle Automático de Iluminação
- **Sensor de presença + luminosidade**: Acende luzes auxiliares automaticamente quando detecta presença no escuro
- **Timer configurável**: Apaga as luzes após tempo configurável sem detecção de movimento (padrão: 20 segundos)

### Detecção de Vazamento de Gás
- **Monitoramento contínuo**: Detecta gases acima do limite configurável
- **Alarme configurável**: Soa alarme após tempo configurável de detecção contínua (padrão: 30 segundos)
- **Notificações**: Envia alertas para familiares via sistema de notificação
- **Segurança**: Para o alarme automaticamente quando o ambiente volta ao normal

### Controle de Temperatura
- **Monitoramento térmico**: Detecta temperaturas extremas (limites configuráveis, padrão: >35°C ou <10°C)
- **Automação climática**: Liga ar condicionado automaticamente na temperatura configurável (padrão: 22°C modo automático)
- **Notificações**: Alerta familiares sobre condições térmicas inadequadas

## Arquitetura

### Componentes
- **ESP32**: Microcontrolador principal para comunicação via MQTT
- **Sensores**: Presença, luminosidade, gases e temperatura
- **Node-RED**: Servidor central para processamento de regras e automação
- **Broker MQTT**: Gerenciamento de filas de comunicação entre dispositivos

### Comunicação
- **Protocolo MQTT**: Comunicação assíncrona entre sensores e servidor
- **Filas específicas**: Cada tipo de sensor possui sua própria fila MQTT
- **Configurações centralizadas**: Limites e parâmetros ajustáveis via Node-RED

### Funcionamento
- ESP32 e sensores atuam como terminais "burros" (apenas coleta e execução)
- Node-RED se inscreve nas filas MQTT específicas de cada sensor
- Processamento centralizado das regras de negócio configuráveis
- Comandos enviados de volta aos dispositivos via MQTT para ações automatizadas


### Diagrama

![Arquitetura em alto nivel](./high-level-architecture.svg)


## Rodando o projeto

### Docker (Recomendado)

A forma mais simples de rodar todo o projeto é usando Docker Compose, que inicializa tanto o MQTT Broker quanto o Node-RED:

```sh
docker compose up -d
```

Isso irá iniciar:
- **Mosquitto MQTT Broker** na porta `1883`
- **Node-RED** na porta `1880`

### MQTT Broker (Mosquitto)

O projeto utiliza o Eclipse Mosquitto como broker MQTT local, rodando em Docker.

**Configuração:**
- Host: `localhost` (ou `mosquitto` para comunicação entre containers)
- Porta: `1883`
- Autenticação: Desabilitada (anonymous access permitido)

**Tópicos MQTT utilizados:**
| Tópico | Descrição |
|--------|-----------|
| `esp32/temperature` | Dados de temperatura e umidade |
| `esp32/gas-leak` | Dados de detecção de vazamento de gás |
| `esp32/lighting` | Dados de iluminação e presença |

**Testando o broker:**
```sh
# Publicar mensagem de teste (requer mosquitto-clients)
mosquitto_pub -h localhost -t "esp32/temperature" -m '{"temp": 25, "umid": 60}'

# Assinar tópico para ver mensagens
mosquitto_sub -h localhost -t "esp32/#" -v
```

### Node-RED

É necessário que o Node-RED seja inicializado com os flows configurados para cada unidade de medida captada e com as dependências necessárias.

O servidor do Node-RED pode ser inicializado de duas maneiras:

Utilizando Docker (junto com o broker MQTT):
```sh
docker compose up -d
```

Utilizando npm com o script criado (requer broker MQTT externo):
```sh
chmod +x ./setup-node-red.sh
./setup-node-red.sh
```

**URLs de acesso:**
- `http://localhost:1880/ui` - acesso aos dashboards com as informações atuais de gás, temperatura e iluminação.
- `http://localhost:1880` - acesso aos flows que captam os dados e disponibilizam no dashboard

### Configuração do ESP32

Para conectar os sensores ESP32 ao broker MQTT local, configure:
- **Broker Host:** IP da máquina rodando o Docker (ex: `192.168.x.x`)
- **Porta:** `1883`

