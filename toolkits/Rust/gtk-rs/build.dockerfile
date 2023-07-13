FROM rust:slim

RUN apt-get update && apt-get install -qq --no-install-recommends libgtk-3-dev build-essential

ADD ./ /workdir
WORKDIR /workdir

RUN cargo clean
RUN cargo build --release

CMD cp ./target/release/app /executable && strip /executable/app

