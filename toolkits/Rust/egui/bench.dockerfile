FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends libx11-6 libxcursor1 libxrandr2 libxi6 libx11-xcb1 libxcb-render0 libxcb-shape0 libxcb-xfixes0 libxkbcommon0 libssl3

CMD /executable/app

