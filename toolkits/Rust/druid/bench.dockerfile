FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends libgtk-3-0

CMD /executable/druid
