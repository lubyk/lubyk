#!/bin/sh
mkdir -p /usr/local/share/lua/5.1/
mkdir -p /usr/local/bin/
cp /Applications/Lubyk/lib/lubyk/lubyk_mac.lua /usr/local/share/lua/5.1/lubyk.lua
cp /Applications/Lubyk/lib/lubyk/lubyk /usr/local/bin/
echo "Lubyk installed."
