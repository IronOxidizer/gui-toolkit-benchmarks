FROM debian:stable-slim

RUN apt-get update && apt-get install -qq default-jre

CMD java -classpath executable app

