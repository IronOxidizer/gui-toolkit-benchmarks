FROM node:slim

RUN apt-get update && apt-get install -qq --no-install-recommends pkg-config libglib2.0-dev libgtk-4-dev libglib2.0-0 libnss3 libatk1.0-0 libatk-bridge2.0-0 libcups2 libdrm2 libgtk-3-0 libgbm1 libasound2 xauth

COPY ./ /workdir
WORKDIR /workdir

RUN npm i

CMD npx electron . --no-sandbox
