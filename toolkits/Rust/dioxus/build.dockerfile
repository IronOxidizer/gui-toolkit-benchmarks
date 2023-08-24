FROM rust:slim

RUN apt-get update && apt-get install -qq --no-install-recommends libwebkit2gtk-4.0-dev libgtk-3-dev libayatana-appindicator3-dev

COPY ./ /workdir
WORKDIR /workdir

RUN cargo clean
RUN cargo build --release

CMD cp ./target/release/app /executable

