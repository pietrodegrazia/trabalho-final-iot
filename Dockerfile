FROM nodered/node-red:latest

# Copiar suas configs (flows, credenciais, settings, nodes instalados etc.)
COPY flows.json /data/flows.json

# # Arquivo de configurações globais do Node-RED
# COPY settings.js /data/settings.js

# instalar nodes extras
RUN npm install --prefix /data node-red-dashboard

# Garante permissões
USER root
RUN chown -R node-red:node-red /data
USER node-red
