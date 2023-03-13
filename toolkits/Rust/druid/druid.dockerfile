FROM rust:slim

RUN apt-get update && apt-get install -qq --no-install-recommends pkg-config libglib2.0-dev libgtk-3-dev

ADD ./ /druid
WORKDIR /druid

RUN cargo clean
RUN cargo build --release

CMD ./target/release/druid

