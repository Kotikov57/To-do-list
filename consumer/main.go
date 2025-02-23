package main

import (
	"context"
	"database/sql"
	"encoding/json"
	"fmt"
	"log"
	"os"
	"time"

	"github.com/segmentio/kafka-go"
	_ "github.com/lib/pq"
)

// Task - структура задачи
type Task struct {
	ID int `json:"id"`
	Name string `json:"name"`
	Status string `json:"status"`
	CreatedAt string `json:"created_at"`
	FinishedAt string `json:"finished_at"`
}

var (
	db       *sql.DB
	kafkaURL = "kafka:9092"
	topic    = "tasks"
	groupID  = "task-consumer-group"
)

func initDB() {
	dsn := os.Getenv("DATABASE_URL")
	var err error
	db, err = sql.Open("postgres", dsn)
	if err != nil {
		log.Fatal("Ошибка подключения к PostgreSQL: ", err)
	}
	if err = db.Ping(); err != nil {
		log.Fatal("Ошибка при проверке соединения:", err)
	}
	log.Println("Подключено к PostgreSQL")
}

func updateTaskStatus(taskIdD int, status string) error {
	query := `UPDATE tasks SET status = $1`
}