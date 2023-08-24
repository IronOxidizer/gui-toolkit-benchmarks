FROM rust:slim

RUN apt-get update && apt-get install -qq --no-install-recommends build-essential cmake pkg-config libfontconfig1-dev libx11-6 libxcursor1 libxrandr-dev libxi6 libx11-xcb-dev libegl-dev

COPY ./ /workdir
WORKDIR /workdir

RUN cargo clean
RUN cargo build --release

CMD cp ./target/release/app /executable/

