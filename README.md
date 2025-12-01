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