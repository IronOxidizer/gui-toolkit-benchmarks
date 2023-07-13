FROM rust:slim

RUN apt-get update && apt-get install -qq --no-install-recommends build-essential cmake git libx11-dev libxext-dev libxft-dev libxinerama-dev libxcursor-dev libxrender-dev libxfixes-dev libpango1.0-dev libcairo2-dev

ADD ./ /workdir
WORKDIR /workdir

RUN cargo clean
RUN cargo build --release

CMD cp ./target/release/app /executable && strip /executable/app

