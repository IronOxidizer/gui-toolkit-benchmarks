FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends default-jdk

ADD ./ /workdir
WORKDIR /workdir

CMD javac -d /executable src/main.java
