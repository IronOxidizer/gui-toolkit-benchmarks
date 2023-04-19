FROM debian:stable-slim

RUN apt-get update && apt-get install --no-install-recommends -qq cmake make g++ pkg-config libfontconfig-dev libx11-6 libxcursor1 libxrandr-dev libxi6 libx11-xcb-dev libegl-dev

CMD /executable/iced
