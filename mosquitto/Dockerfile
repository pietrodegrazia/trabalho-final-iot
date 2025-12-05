FROM eclipse-mosquitto:2

# Copia a configuração personalizada
COPY config/mosquitto.conf /mosquitto/config/mosquitto.conf

# Cria diretórios para dados e logs
RUN mkdir -p /mosquitto/data /mosquitto/log && \
    chown -R mosquitto:mosquitto /mosquitto

# Expõe as portas MQTT (TCP) e WebSocket
EXPOSE 1883 9001

# Comando padrão
CMD ["mosquitto", "-c", "/mosquitto/config/mosquitto.conf"]

