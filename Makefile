CC = clang
C_FLAGS = -O3

all: compile

compile: $(wildcard ./*.c)
	$(CC) $(CC_FLAGS) $? -o ./print
