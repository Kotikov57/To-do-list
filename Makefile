COMP = g++
COMPFLAGS = -Wall -Wextra -Iinclude
TARGET = main
LDFLAGS = -lsqlite3

all: $(TARGET)

$(TARGET): database.o server.o task_manager.o main.o
	$(COMP) $(COMPFLAGS)  -o $@ $^ $(LDFLAGS)

database.o: src/database.cpp
	$(COMP) $(COMPFLAGS) $(LDFLAGS) -c $< -o $@

server.o: src/server.cpp
	$(COMP) $(COMPFLAGS) -c $< -o $@

task_manager.o: src/task_manager.cpp
	$(COMP) $(COMPFLAGS) -c $< -o $@

main.o: src/main.cpp
	$(COMP) $(COMPFLAGS)  -c $< -o $@ 

clean:
	rm -rf *.o $(TARGET)
