FROM alpine:3.13

RUN apk add --no-cache  \
    cmake \
    g++ \
    gcc \
    git \
    make \
    musl-dev \
    musl \
    openssh

RUN mkdir /code  \
    && git clone https://github.com/prodbymozart/maze-generator.git /code \
    && cd /code \
    && cmake . \
    && make

WORKDIR /code

ENTRYPOINT [ "./TiledMazeGenerator" ]
CMD [ "./TiledMazeGenerator" ]
