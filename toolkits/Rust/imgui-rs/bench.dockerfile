FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends libfontconfig1 libx11-6 libxcursor1 libxrandr2 libxi6 libx11-xcb1 libgl1-mesa-dri libegl1


CMD /executable/app
