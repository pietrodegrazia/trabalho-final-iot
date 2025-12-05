#!/bin/sh

# Substitui a variável MQTT_BROKER no flows.json
# Se MQTT_BROKER não estiver definido, usa "mosquitto" (padrão para Docker local)
BROKER_HOST="${MQTT_BROKER:-mosquitto}"

echo "Configurando MQTT Broker: $BROKER_HOST"

# Substitui o placeholder no flows.json
sed -i "s/\${MQTT_BROKER}/$BROKER_HOST/g" /data/flows.json

# Inicia o Node-RED
exec npm start -- --userDir /data

