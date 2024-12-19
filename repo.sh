#!/bin/bash
if [ -f .env ]; then
  source .env
else
  echo ".env file not found! Please create one with the required parameters."
  exit 1
fi
if [ -z "$USERNAME" ] || [ -z "$TOKEN" ] || [ -z "$REPO_NAME" ]; then
  echo "Error: Missing required parameters in .env file (USERNAME, TOKEN, REPO_NAME)."
  exit 1
fi
if [ "$PRIVATE" == "private" ]; then
  VISIBILITY=true
else
  VISIBILITY=false
fi
echo "Creating repository $REPO_NAME on GitHub..."
curl -u "$USERNAME:$TOKEN" https://api.github.com/user/repos -d "{\"name\":\"$REPO_NAME\", \"private\":$VISIBILITY}"
if [ $? -eq 0 ]; then
  echo "Repository $REPO_NAME created successfully!"
else
  echo "Failed to create repository $REPO_NAME."
  exit 1
fi
if [ -d ".git" ]; then
  echo "Removing existing .git directory..."
  rm -rf .git
fi
echo "Initializing new Git repository..."
git init
git remote add origin https://github.com/$USERNAME/$REPO_NAME.git
echo "Remote origin set to https://github.com/$USERNAME/$REPO_NAME.git"

cat > README.md <<EOF
# ${REPO_NAME}

Benvenuto nel repository ${REPO_NAME}!

## Descrizione

Aggiungi qui una descrizione del progetto.

## Istruzioni

Aggiungi qui le istruzioni per utilizzare il progetto.
EOF
echo "REPO_NAME=${REPO_NAME}" > .github/workflows/.env
echo "Done! You can now add files, commit changes, and push to the new repository."
