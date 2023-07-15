FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends libqt5widgets5 libqt5qml5 libqt5quick5 qml-module-qtquick-controls2 qml-module-qtquick2 qml-module-qtquick-window2

CMD /executable/app
