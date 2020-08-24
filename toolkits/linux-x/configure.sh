#!/usr/bin/env sh

git pull

sudo apt-get install -y curl

start_dir=$PWD
for i in *
do
	if [ $i != README.md -a $i != configure.sh ]
	then
		cd $start_dir/$i
		./configure.sh
	fi
done

