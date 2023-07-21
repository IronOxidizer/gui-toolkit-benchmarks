FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends libwxgtk3.2-1

CMD /executable/app
