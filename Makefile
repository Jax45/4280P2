CC	= g++
CFLAGS	= -Wall -g

MASTER_SRC = main.cpp 
SRC = scanner.h parser.h tree.h
SHARE_OBJ = scanner.o parser.o tree.o
AUX = fsaTable.h node.h
CP11 = -std=c++11
MASTER_OBJ = $(MASTER_SRC:.cpp=.o)
OBJ	= $(SRC:.h=.o)

TARGET	= P2

OUTPUT = $(TARGET)
all: $(OUTPUT)

%.o: %.cpp $(SRC) $(AUX)
	$(CC) $(CP11) $(CFLAGS) -c $< -o $@

$(TARGET): $(MASTER_OBJ) $(SHARE_OBJ)
	$(CC) $(CP11) $(CFLAGS) $(MASTER_OBJ) $(OBJ) -o $(TARGET)


clean:
	/bin/rm -f *.o $(TARGET)
clear:
	/bin/rm -f *.preorder *.postorder *.inorder
