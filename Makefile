PREFIX=/usr/local

ifdef GTEST_DIR
	GTEST_DIR:=$(shell readlink -f "$(GTEST_DIR)" | sed 's/ /\\ /')
endif

all: test-all

clean:
	make clean -C test

install:
	cp bin/cmock-config $(PREFIX)/bin
	cp -r include/cmock $(PREFIX)/include

test:
	make run -C test

test-all:
	make -C test

uninstall:
	rm $(PREFIX)/bin/cmock-config
	rm -r $(PREFIX)/include/cmock

.PHONY: clean install test
