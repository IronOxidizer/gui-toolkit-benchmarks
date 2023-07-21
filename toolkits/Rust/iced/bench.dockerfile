FROM debian:stable-slim

RUN apt-get update && apt-get install --no-install-recommends -qq libfontconfig1 libx11-6 libxcursor1 libxrandr2 libxi6 libx11-xcb1 libegl1

CMD /executable/app
