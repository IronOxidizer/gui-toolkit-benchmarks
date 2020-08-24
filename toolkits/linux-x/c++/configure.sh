#!/usr/bin/env sh

sudo apt-get install -y make g++

start_dir=$PWD
for i in *
do
	if [ $i != README.md -a $i != configure.sh -a -f $start_dir/$i ]
	then
		cd $start_dir/$i
		./configure.sh
	fi
done
