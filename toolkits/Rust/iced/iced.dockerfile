FROM rust:slim

#RUN apt-get update && apt-get install -qq pkg-config libglib2.0-dev libgtk-3-dev

ADD ./ /iced
WORKDIR /iced

RUN cargo clean
RUN cargo build --release

CMD ./target/release/iced
