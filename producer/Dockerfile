FROM golang AS builder
WORKDIR /app
COPY go.mod go.sum ./
RUN go mod download
COPY . .
RUN go build -o producer main.go
FROM ubuntu
WORKDIR /app
RUN apt-get update && apt-get install -y ca-certificates && rm -rf /var/lib/apt/lists/*
COPY --from=builder /app/producer /app/producer
EXPOSE 8080
CMD ["/app/producer"]