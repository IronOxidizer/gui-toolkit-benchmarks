FROM rust:slim

ADD ./ /druid
WORKDIR /druid

RUN cargo clean
RUN RUSTFLAGS="-C target-cpu=native" cargo build --release

EXPOSE 8080

CMD ./target/release/druid
