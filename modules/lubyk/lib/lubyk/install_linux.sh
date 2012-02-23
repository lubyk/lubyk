#!/bin/sh
# Must be run from the git/lubyk directory.
mkdir -p /usr/local/share/lua/5.1/
mkdir -p /usr/local/bin/
cp deploy/linux/lib/lubyk_linux.lua /usr/local/share/lua/5.1/lubyk.lua
cp /usr/bin/lua5.1 /usr/local/bin/lubyk
echo "Lubyk installed."

