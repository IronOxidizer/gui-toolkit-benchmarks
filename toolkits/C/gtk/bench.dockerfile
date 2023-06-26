FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends libgtk-4-1

CMD /executable/app
