OBJS = mirror_client.o functions.o lists.o
SOURCE = mirror_client.c functions.c lists.c
HEADER = functions.h structs.h
DIRS = common *_mirror
OUT = mirror_client
CC = gcc
FLAGS = -g3 -c

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

mirror_client.o: mirror_client.c
	$(CC) $(FLAGS) mirror_client.c

functions.o: functions.c
	$(CC) $(FLAGS) functions.c

lists.o: lists.c
	$(CC) $(FLAGS) lists.c

clean:
	rm -f $(OBJS) $(OUT) log_file*
	rm -r $(DIRS)
