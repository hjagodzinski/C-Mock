CMOCK=$(shell readlink -f ..)
CWD=$(shell pwd)

CFLAGS=$(shell $(CMOCK)/bin/cmock-config --cflags)
LDFLAGS=$(shell $(CMOCK)/bin/cmock-config --libs $(GMOCK) $(GTEST))

SRCS=$(shell ls *.cc)
OBJS=$(SRCS:.cc=.o)

all: libfoo.so cmock_test

foo.o: foo.c
	$(CC) -c -fPIC $^ -o $@

libfoo.so: foo.o
	$(CC) -shared -Wl,-soname,$(CWD)/$@ -o $@ $^

%.o: %.cc
	$(CXX) -c $(CFLAGS) $^ -o $@

cmock_test: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) -pthread -L$(CWD) -lfoo -lgmock_main -lgmock -lgtest

clean:
	rm -f $(OBJS) cmock_test foo.o libfoo.so
