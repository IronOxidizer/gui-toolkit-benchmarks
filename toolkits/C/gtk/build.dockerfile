FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends pkg-config libglib2.0-dev libgtk-4-dev

ADD ./ /workdir
WORKDIR /workdir

CMD gcc $(pkg-config --cflags gtk4) -o /executable/app src/main.c $(pkg-config --libs gtk4) -O3 && strip /executable/app
