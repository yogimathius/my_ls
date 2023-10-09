CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = my_ls.c sort_lists.c sort_lists_time.c check_list.c
OBJS = $(SRCS:.c=.o)
HEADER = sort_header.h

TARGET = my_ls

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)
