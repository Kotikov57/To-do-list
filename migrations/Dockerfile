FROM golang
WORKDIR /app
RUN apt update && apt install -y curl && \
    curl -L https://github.com/golang-migrate/migrate/releases/download/v4.16.0/migrate.linux-amd64.tar.gz | tar xvz && \
    mv migrate /usr/local/bin/
COPY . /migrations
CMD ["migrate", "-path", "/migrations", "-database", "postgres://postgres:fkla5283@db:5432/list?sslmode=disable","up"]