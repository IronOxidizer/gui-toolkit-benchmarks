#!/usr/bin/env sh

sudo apt-get install -y gcc

curl -sSf https://sh.rustup.rs/ | sh -s -- -y

PATH="$PATH:$HOME/.cargo/bin"

echo $PATH
