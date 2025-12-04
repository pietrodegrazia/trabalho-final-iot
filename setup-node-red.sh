#!/bin/bash
# filepath: setup-nodered.sh

set -e  # Para o script se houver erro

echo "🚀 Instalando Node-RED e configurando o ambiente..."

# Verifica se o Node.js está instalado
if ! command -v node &> /dev/null; then
    echo "❌ Node.js não está instalado. Instalando via Homebrew..."
    brew install node
fi

# Verifica se o npm está instalado
if ! command -v npm &> /dev/null; then
    echo "❌ npm não está instalado."
    exit 1
fi

echo "✅ Node.js $(node -v) e npm $(npm -v) encontrados"

# Define o diretório de dados do Node-RED
NODE_RED_DIR="$HOME/.node-red"
mkdir -p "$NODE_RED_DIR"

# Instala o Node-RED globalmente se não estiver instalado
if ! command -v node-red &> /dev/null; then
    echo "📦 Instalando Node-RED globalmente..."
    npm install -g --unsafe-perm node-red
else
    echo "✅ Node-RED já está instalado: $(node-red --version)"
fi

# Navega para o diretório do Node-RED
cd "$NODE_RED_DIR"

# Instala o node-red-dashboard
echo "📦 Instalando node-red-dashboard..."
npm install node-red-dashboard

# Copia o arquivo flows.json
WORKSPACE_DIR="/Users/i559431/unisinos/iot/gb/trabalho-final-iot"
if [ -f "$WORKSPACE_DIR/flows.json" ]; then
    echo "📋 Copiando flows.json..."
    cp "$WORKSPACE_DIR/flows.json" "$NODE_RED_DIR/flows.json"
else
    echo "⚠️  flows.json não encontrado em $WORKSPACE_DIR"
fi

# Copia settings.js se existir
if [ -f "$WORKSPACE_DIR/settings.js" ]; then
    echo "⚙️  Copiando settings.js..."
    cp "$WORKSPACE_DIR/settings.js" "$NODE_RED_DIR/settings.js"
fi

echo ""
echo "✅ Configuração concluída!"
echo ""
echo "Para iniciar o Node-RED, execute:"
echo "  node-red"
echo ""
echo "Depois acesse: http://localhost:1880"
echo ""
echo "Para parar o Node-RED, pressione Ctrl+C"