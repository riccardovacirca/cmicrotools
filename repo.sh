#!/bin/bash

# Carica i parametri dal file .env
if [ -f .env ]; then
  source .env
else
  echo ".env file not found! Please create one with the required parameters."
  exit 1
fi

# Controlla se USERNAME, TOKEN e REPO_NAME sono settati
if [ -z "$USERNAME" ] || [ -z "$TOKEN" ] || [ -z "$REPO_NAME" ]; then
  echo "Error: Missing required parameters in .env file (USERNAME, TOKEN, REPO_NAME)."
  exit 1
fi

# Configura la visibilità del repository
if [ "$PRIVATE" == "private" ]; then
  VISIBILITY=true
else
  VISIBILITY=false
fi

# Crea il repository usando l'API GitHub
curl -u "$USERNAME:$TOKEN" https://api.github.com/user/repos -d "{\"name\":\"$REPO_NAME\", \"private\":$VISIBILITY}"

# Configura il repository locale
if [ $? -eq 0 ]; then
  echo "Repository $REPO_NAME created successfully!"
  git remote add origin https://github.com/$USERNAME/$REPO_NAME.git
  echo "Remote origin set to https://github.com/$USERNAME/$REPO_NAME.git"
else
  echo "Failed to create repository $REPO_NAME."
fi
