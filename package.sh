#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <package-version>"
  exit 1
fi

if [ -f .env ]; then
  source .env
else
  echo ".env file not found! Please create one with the required parameters."
  exit 1
fi

if [ -z "$REPO_NAME" ]; then
  echo "Error: Missing REPO_NAME in .env file."
  exit 1
fi

PACKAGE_NAME="$REPO_NAME"
PACKAGE_VERSION="$1"
EXECUTABLE_NAME="$REPO_NAME"
EXECUTABLE_PATH="/usr/local/bin/cm"
OUTPUT_DIR="/tmp/$REPO_NAME-package"
DEBIAN_DIR="$OUTPUT_DIR/DEBIAN"
BIN_DIR="$OUTPUT_DIR/usr/local/bin"

mkdir -p "$BIN_DIR"
mkdir -p "$DEBIAN_DIR"

cp "$EXECUTABLE_PATH" "$BIN_DIR/$EXECUTABLE_NAME"
chmod +x "$BIN_DIR/$EXECUTABLE_NAME"

cat > "$DEBIAN_DIR/control" <<EOL
Package: $REPO_NAME
Version: $PACKAGE_VERSION
Architecture: amd64
Maintainer: $AUTHOR<$EMAIL>
Description: $DESCRIPTION
Depends: libc6 (>= 2.30)
Priority: optional
Section: utils
Homepage: $URL
EOL

dpkg-deb --build "$OUTPUT_DIR"
rm -rf "$OUTPUT_DIR"

echo "Done."
