all: test/cmock_test

test:
	./test/cmock_test

test/cmock_test:
	make -C test

clean:
	make clean -C test

.PHONY: clean test
