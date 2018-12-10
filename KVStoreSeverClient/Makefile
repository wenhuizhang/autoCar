# Top level makefile to build and test everything below it
DEPS = lib server client
TEST_DEPS = test_lib test_server test_client

.PHONY: all test $(DEPS) $(TEST_DEPS)
all: $(DEPS)

rebuild: clean all

test: $(TEST_DEPS)

lib:
	cd lib && make

server:
	cd server && make

client:
	cd client && make

test_lib:
	cd lib && make test

test_client:
	cd client && make test

test_server:
	cd server && make test

clean:
	cd lib && make clean
	cd server && make clean
	cd client && make clean
