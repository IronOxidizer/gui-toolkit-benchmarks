#!/usr/bin/env sh

curl -sSf https://sh.rustup.rs/ | sh -s -- -y

PATH="$PATH:$HOME/.cargo/bin"

echo $PATH
