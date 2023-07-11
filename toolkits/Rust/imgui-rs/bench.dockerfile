FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends build-essential cmake pkg-config libfontconfig1-dev libx11-6 libxcursor1 libxrandr-dev libxi6 libx11-xcb-dev libegl-dev


CMD /executable/app
