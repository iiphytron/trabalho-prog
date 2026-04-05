CC = gcc
CFLAGS = -Wall -O3
TARGET = eliza
SRCS = main.c base_dados.c asterisco.c funcoes.c
OBJS = $(SRCS:.c=.o)
DEPS = base_dados.h asterisco.h funcoes.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@  $

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
