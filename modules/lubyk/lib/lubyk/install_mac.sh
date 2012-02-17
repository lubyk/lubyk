#!/bin/sh
mkdir -p /usr/local/share/lua/5.1/
mkdir -p /usr/local/bin/
cp /Applications/Lubyk/lib/lubyk.lua /usr/local/share/lua/5.1/
cp /Applications/Lubyk/Lubyk.app/Contents/Resources/lubyk /usr/local/bin/
echo "Lubyk installed."
