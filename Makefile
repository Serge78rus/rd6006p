TARGET = rd6006p
PREFIX = /usr/local/bin
SRCS = main.c rd6006p.c options.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -O2 -Wall -lmodbus

.PHONY: all clean install uninstall

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

.c.o:
	$(CC) $(CFLAGS)  -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJS)
install:
	install $(TARGET) $(PREFIX)
uninstall:
	rm -rf $(PREFIX)/$(TARGET)
