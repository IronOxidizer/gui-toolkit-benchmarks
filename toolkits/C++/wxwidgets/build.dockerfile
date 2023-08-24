FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends build-essential g++ libwxgtk3.2-dev

COPY ./ /workdir
WORKDIR /workdir

CMD g++ -s `wx-config --cflags` -o /executable/app src/main.cpp `wx-config --libs` -O2
