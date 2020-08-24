#!/usr/bin/env sh

sudo apt-get install -y gcc

curl -sSf https://sh.rustup.rs/ | sh -s -- -y

PATH="$PATH:$HOME/.cargo/bin"

start_dir=$PWD
for i in *
do
	if [ $i != README.md -a $i != configure.sh ]
	then
		cd $start_dir/$i
		./configure.sh
	fi
done
