FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends build-essential libfltk1.3-dev libx11-dev libxext-dev libxft-dev libxinerama-dev libxcursor-dev libxrender-dev libxfixes-dev libpango1.0-dev libcairo2-dev

COPY ./ /workdir
WORKDIR /workdir

CMD g++ -s -std=c++17 $(fltk-config --cflags) -o /executable/app src/main.cpp $(fltk-config --libs --ldflags) -O2
