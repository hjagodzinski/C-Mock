CMOCK=$(shell readlink -f ..)
CWD=$(shell pwd)

CFLAGS=$(shell $(CMOCK)/bin/cmock-config --cflags $(GTEST_DIR))
LDFLAGS=$(shell $(CMOCK)/bin/cmock-config --libs $(GTEST_DIR))

SRCS=$(shell ls *.cc)
EXECS=$(SRCS:.cc=)

all: $(EXECS)

run:
	./cmock-function-class-mockers_test
	./cmock-function-mockers_test
	./cmock-function-class-mockers-old-style_test
	./cmock-spec-builders_test

math.o: math.c
	$(CC) -c -fPIC $^ -o $@

libmath.so: math.o
	$(CC) -shared -Wl,-soname,$(CWD)/$@ -o $@ $^

%.o: %.cc
	$(CXX) -c $(CFLAGS) $^ -o $@

%_test: %_test.o libmath.so
	$(CXX) -o $@ $^ $(LDFLAGS) -pthread -L$(CWD) -lmath -lgmock_main -lgmock -lgtest

clean:
	rm -f *.o *.so $(EXECS)
