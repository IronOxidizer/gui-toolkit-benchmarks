FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends pkg-config libglib2.0-dev libgtk-4-dev

COPY ./ /workdir
WORKDIR /workdir

CMD gcc -s $(pkg-config --cflags gtk4) -o /executable/app src/main.c $(pkg-config --libs gtk4) -O2
