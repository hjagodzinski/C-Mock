CMOCK=$(shell readlink -f ..)
CWD=$(shell pwd)

CFLAGS=$(shell $(CMOCK)/bin/cmock-config --cflags)
LDFLAGS=$(shell $(CMOCK)/bin/cmock-config --libs $(GMOCK) $(GTEST))

SRCS=$(shell ls *.cc)
OBJS=$(SRCS:.cc=.o)

all: libmath.so cmock_test

math.o: math.c
	$(CC) -c -fPIC $^ -o $@

libmath.so: math.o
	$(CC) -shared -Wl,-soname,$(CWD)/$@ -o $@ $^

%.o: %.cc
	$(CXX) -c $(CFLAGS) $^ -o $@

cmock_test: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) -pthread -L$(CWD) -lmath -lgmock_main -lgmock -lgtest

clean:
	rm -f $(OBJS) cmock_test math.o libmath.so
