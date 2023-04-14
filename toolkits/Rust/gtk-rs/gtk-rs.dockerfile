FROM rust:slim

RUN apt-get update && apt-get install -qq --no-install-recommends libgtk-3-dev build-essential

ADD ./ /gtk-rs
WORKDIR /gtk-rs

RUN cargo clean
RUN cargo build --release

CMD ./target/release/gtk-rs

