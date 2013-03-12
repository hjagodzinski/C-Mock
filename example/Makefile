CMOCK=$(shell readlink -f ..)
CWD=$(shell pwd)

CFLAGS=$(shell $(CMOCK)/bin/cmock-config --cflags)
LDFLAGS=$(shell $(CMOCK)/bin/cmock-config --libs)

all: libfoo.so example

foo.o: foo.c
	$(CC) -c -fPIC $^ -o $@

libfoo.so: foo.o
	$(CC) -shared -Wl,-soname,$(CWD)/$@ -o $@ $^

example.o: example.cc
	$(CXX) -c $(CFLAGS) $^ -o $@

example: example.o
	$(CXX) -o $@ $< $(LDFLAGS) -pthread -L$(CWD) -lfoo -lgmock_main -lgmock -lgtest

clean:
	rm -f example.o example foo.o libfoo.so
