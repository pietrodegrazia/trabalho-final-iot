FROM nodered/node-red:latest

# Copiar suas configs (flows, credenciais, settings, nodes instalados etc.)
COPY flows.json /data/flows.json

# Copiar script de inicialização
COPY docker-entrypoint.sh /docker-entrypoint.sh

# instalar nodes extras
RUN npm install --prefix /data node-red-dashboard

# Garante permissões
USER root
RUN chown -R node-red:node-red /data && \
    chmod +x /docker-entrypoint.sh
USER node-red

# Usa o script de entrada personalizado
ENTRYPOINT ["/docker-entrypoint.sh"]
