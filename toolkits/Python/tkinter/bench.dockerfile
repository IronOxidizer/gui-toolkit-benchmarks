FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends python3 python3-tk libtk8.6

COPY src/main.py /workdir/main.py
WORKDIR /workdir

CMD python3 main.py
