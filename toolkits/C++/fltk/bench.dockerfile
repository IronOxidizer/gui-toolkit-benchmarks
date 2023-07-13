FROM debian:stable-slim

RUN apt-get update && apt-get install -qq --no-install-recommends libx11-6 libxinerama1 libxft2 libxext6 libxcursor1 libxrender1 libxfixes3 libcairo2 libpango-1.0-0 libpangocairo-1.0-0 libpangoxft-1.0-0 libglib2.0-0 libfontconfig1 libfltk1.3-dev

CMD /executable/app
