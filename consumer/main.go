package main

import (
	"context"
	"database/sql"
	"encoding/json"
	"log"
	"os"
	"time"
	"github.com/segmentio/kafka-go"
	_ "github.com/lib/pq"
)

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

func updateTaskStatus(taskID int, status string) error {
	var err error
	if status == "Completed" {
		query := `UPDATE tasks SET status = $1, finisged_at = $2 WHERE id = $3`
		_, err = db.Exec(query,status,time.Now(),taskID)
	} else{
		query := `UPDATE tasks SET status = $1 WHERE id = $2`
		_, err = db.Exec(query,status,taskID)
	}
	return err
}

func consumeMessages() {
	r := kafka.NewReader(kafka.ReaderConfig{
		Brokers: []string{kafkaURL},
		Topic: topic,
		GroupID: groupID,
		MaxBytes: 10e6,
	})
	log.Println("Consumer запущен, слушает Kafka")
	for {
		msg, err := r.ReadMessage(context.Background())
		if err != nil {
			log.Println("Ошибка чтения Kafka", err)
			continue
		}
		var task Task
		if err := json.Unmarshal(msg.Value, &task); err != nil {
			log.Println("Ошибка парсинга JSON", err)
			continue
		}
		log.Printf("Получена задача $+v\n", task)
		if err := updateTaskStatus(task.ID, "Processing"); err != nil {
			log.Println("Ошибка обновления статуса", err)
			continue
		}
		log.Printf("Задача %d обновлена в статус 'Proccessing'\n", task.ID)
		time.Sleep(5 * time.Second)
		if err := updateTaskStatus(task.ID, "Completed"); err != nil {
			log.Println("Ошибка обновления статуса", err)
			continue
		}
		log.Printf("Задача %d завершена", task.ID)
	}
}

func main() {
	initDB()
	consumeMessages()
}