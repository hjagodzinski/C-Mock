PREFIX=/usr/local

all: test/cmock_test

clean:
	make clean -C test

install:
	cp bin/cmock-config $(PREFIX)/bin
	cp -r include/cmock $(PREFIX)/include

test:
	./test/cmock_test

test/cmock_test:
	make -C test

uninstall:
	rm $(PREFIX)/bin/cmock-config
	rm -r $(PREFIX)/include/cmock

.PHONY: clean install test
