FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends build-essential qtbase5-dev libqt5widgets5 qtdeclarative5-dev libqt5qml5 libqt5quick5 qml-module-qtquick-controls2 qml-module-qtquick2 qml-module-qtquick-window2

COPY ./ /workdir
WORKDIR /workdir

# CMD g++ std=c++11 fPIC $(pkg-config --cflags Qt5Widgets) -o /executable/app src/main.cpp $(pkg-config --libs Qt5Widgets)
CMD qmake app.pro && make
