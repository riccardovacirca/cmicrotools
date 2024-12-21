#!/bin/bash

# URL del servizio
URL="http://localhost:2310/api/helloworld"

echo "Eseguo il test sull'endpoint $URL..."

# Effettua una richiesta HTTP GET all'endpoint
RESPONSE=$(curl -s -o /dev/null -w "%{http_code}" "$URL")

# Verifica il codice di risposta
if [ "$RESPONSE" -eq 200 ]; then
  echo "Test superato: il servizio ha risposto con HTTP 200"
  exit 0
else
  echo "Test fallito: il servizio ha risposto con HTTP $RESPONSE"
  exit 1
fi
