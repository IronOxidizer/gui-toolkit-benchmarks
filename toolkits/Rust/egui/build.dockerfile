FROM rust:slim

RUN apt-get update && apt-get install -qq --no-install-recommends libx11-6 libxcursor1 libxrandr-dev libxi6 libx11-xcb1 libclang-dev libgtk-3-dev libxcb-render0-dev libxcb-shape0-dev libxcb-xfixes0-dev libxkbcommon-dev libssl-dev

ADD ./ /workdir
WORKDIR /workdir

RUN cargo clean
RUN cargo build --release

CMD cp ./target/release/app /executable && strip /executable/app

