CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = 537make
SCAN_BUILD_DIR = scan-build-out

all: main.o LinkedList.o GraphNode.o constants.o Traversal.o
	$(CC) -o $(EXE) main.o LinkedList.o GraphNode.o constants.o Traversal.o

clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

main.o: main.c LinkedList.h GraphNode.h constants.h Traversal.h
	$(CC) $(WARNING_FLAGS) -c main.c

LinkedList.o: LinkedList.c LinkedList.h
	$(CC) $(WARNING_FLAGS) -c LinkedList.c

GraphNode.o: GraphNode.c GraphNode.h LinkedList.h constants.h
	$(CC) $(WARNING_FLAGS) -c GraphNode.c

constants.o: constants.c constants.h
	$(CC) $(WARNING_FLAGS) -c constants.c

Traversal.o: Traversal.c Traversal.h LinkedList.h GraphNode.h
	$(CC) $(WARNING_FLAGS) -c Traversal.c

#
# Run the Clang Static Analyzer
#
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

#
# View the one scan available using firefox
#
scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html