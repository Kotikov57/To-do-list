FROM gcc:latest

WORKDIR /app
COPY . /app

RUN apt-get update && apt-get install -y \
    libboost-all-dev sqlite3 libsqlite3-dev libasio-dev \
    libpoco-dev

RUN make

CMD ["./main"]