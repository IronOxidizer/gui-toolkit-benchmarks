FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends libx11-6 libxcursor1 libxrandr-dev libxi6 libx11-xcb1 libclang-dev libgtk-3-dev libxcb-render0-dev libxcb-shape0-dev libxcb-xfixes0-dev libxkbcommon-dev libssl-dev

CMD /executable/egui

