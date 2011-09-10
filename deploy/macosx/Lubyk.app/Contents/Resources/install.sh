#!/bin/bash
# Must be run as root
if [[ $EUID -ne 0 ]]; then
  echo "Install script must be run as root" 2>&1
  exit 1
fi

REZ_PATH="/Applications/Lubyk.app/Contents/Resources"

# install lua
if [[ -f /usr/bin/lua ]]; then
  echo "Lua already installed"
else
  echo "Installing /usr/bin/lua..."
  cp $REZ_PATH/lua /usr/bin/
  if [[ -f /usr/bin/lua ]]; then
    echo "OK"
  else
    echo "Failed !"
    exit 1
  fi
fi

# install lubyk command
echo "Installing /usr/bin/lubyk..."
cp $REZ_PATH/lubyk /usr/bin/
if [[ -f /usr/bin/lubyk ]]; then
  echo "OK"
else
  echo "Failed !"
  exit 1
fi

# install lubyk loader script
test -e /usr/local || mkdir /usr/local
test -e /usr/local/lib || mkdir /usr/local/lib
test -e /usr/local/lib/lua || mkdir /usr/local/lib/lua
test -e /usr/local/lib/lua/5.1 || mkdir /usr/local/lib/lua/5.1
echo "Installing /usr/local/lib/lua/5.1/lubyk.lua..."
cp $REZ_PATH/lubyk.lua /usr/local/lib/lua/5.1/
if [[ -f /usr/local/lib/lua/5.1/lubyk.lua ]]; then
  echo "OK"
else
  echo "Failed !"
  exit 1
fi

echo "****************************************"
echo "Successfully installed lubyk."
echo ""
echo "Have fun!"
echo "****************************************"
