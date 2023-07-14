FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends libwebkit2gtk-4.1-0 libgtk-3-0 libayatana-appindicator3-1

CMD /executable/app
