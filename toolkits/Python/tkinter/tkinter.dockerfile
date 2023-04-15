FROM python:slim

RUN apt-get update && apt-get install -qq --no-install-recommends libtk8.6

ADD ./ /build
WORKDIR /build

#RUN pip3 install -U pip

CMD python3 main.py

