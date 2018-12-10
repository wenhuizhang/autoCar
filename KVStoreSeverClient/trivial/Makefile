#Use the gcc compiler
CC = gcc
CFLAGS =
DEPS =
LDFLAGS =
#Objects created by makefile
OBJS = trivial-server trivial-client

#Ensure compiling is done with all necessary dependencies
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJS)

rebuild: clean all


trivial-server: trivial-server.c
	$(CC) $(CFLAGS) -o $@ $^

trivial-client: trivial-client.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@rm -f $(OBJS)
