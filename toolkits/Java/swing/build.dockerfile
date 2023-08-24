FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends default-jdk

COPY ./ /workdir
WORKDIR /workdir

CMD javac -d /executable src/app.java
