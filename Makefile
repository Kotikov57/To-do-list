COMP = g++
COMPFLAGS = -Wall -Wextra -Iinclude
TARGET = main
LDFLAGS = -lsqlite3

all: $(TARGET)

$(TARGET): database.o server.o task_manager.o main.o
	$(COMP) $(COMPFLAGS) -o $@ $^

database.o: src/database.cpp
	$(COMP) $(COMPFLAGS) -o $@ $^ $(LDFLAGS)

server.o: src/server.cpp
	$(COMP) $(COMPFLAGS) -o $@ $^

task_manager.o: src/task_manager.cpp
	$(COMP) $(COMPFLAGS) -o $@ $^

main.o: src/main.cpp
	$(COMP) $(COMPFLAGS) -o $@ $^

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
