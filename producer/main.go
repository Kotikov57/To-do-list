package main

import (
	"context"
	"database/sql"
	"encoding/json"
	"log"
	"net/http"
	"os"
	"github.com/gin-gonic/gin"
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
	db *sql.DB
	kafkaURL=os.Getenv("KAFKA_PORT")
	topic = "tasks"
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

func sendToKafka(task Task) error {
	w := kafka.NewWriter(kafka.WriterConfig{
		Brokers: []string{kafkaURL},
		Topic: topic,
		Balancer: &kafka.LeastBytes{},
	})
	defer w.Close()
	message, _ := json.Marshal(task)
	err := w.WriteMessages(context.Background(), kafka.Message{Value: message})
	return err
}

func createTask(c *gin.Context) {
	var task Task
	if err := c.ShouldBindJSON(&task); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error":"Неверный формат данных"})
		return
	}

	query := `INSERT INTO tasks (name, status) VALUES ($1,$2) RETURNING id, created_at`
	err := db.QueryRow(query, task.Name, "Pending").Scan(&task.ID, &task.CreatedAt)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error":"Ошибка сохранения новой задачи"})
		return
	}
	if err := sendToKafka(task); err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error":"Ошибка отправки в Kafka"})
		return
	}
	c.JSON(http.StatusOK, gin.H{"message":"Задача создана","task":task})
}

func main() {
	initDB()
	r:=gin.Default()
	r.POST("/task", createTask)
	log.Println("Producer запущен на порту 8080")
	r.Run(":8080")
}