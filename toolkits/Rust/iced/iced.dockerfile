FROM rust:slim

ADD ./ /iced
WORKDIR /iced

RUN cargo clean
RUN RUSTFLAGS="-C target-cpu=native" cargo build --release

EXPOSE 8080

CMD ./target/release/iced
