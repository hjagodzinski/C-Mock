CMOCK=$(shell readlink -f ../include)
CWD=$(shell pwd)

all: libfoo.so example

foo.o: foo.c
	$(CC) -c -fPIC $^ -o $@

libfoo.so: foo.o
	$(CC) -shared -Wl,-soname,$(CWD)/$@ -o $@ $^

example.o: example.cc
	$(CXX) -c -I$(CMOCK) $^ -o $@

example: example.o
	$(CXX) -o $@ $< -rdynamic -pthread -Wl,--no-as-needed -ldl -L$(CWD) -lfoo -lgmock_main -lgmock -lgtest

clean:
	rm -f example.o example foo.o libfoo.so
