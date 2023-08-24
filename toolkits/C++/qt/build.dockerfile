FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends build-essential qtbase5-dev libqt5widgets5

COPY ./ /workdir
WORKDIR /workdir

# CMD g++ std=c++11 fPIC $(pkg-config --cflags Qt5Widgets) -o /executable/app src/main.cpp $(pkg-config --libs Qt5Widgets)
CMD qmake app.pro && make
