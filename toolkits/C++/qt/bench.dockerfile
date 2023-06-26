FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends libqt5widgets5

CMD /executable/app
