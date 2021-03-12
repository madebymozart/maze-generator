FROM alpine:3.13

RUN apk add --no-cache  \
    cmake \
    g++ \
    gcc \
    git \
    make \
    musl

ADD . /code

RUN cd /code \
    && cmake . \
    && make

WORKDIR /code

ENTRYPOINT [ "./TiledMazeGenerator" ]
CMD [ "./TiledMazeGenerator" ]
