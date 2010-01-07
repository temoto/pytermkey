PYTHON           = python
#PYTHON           = $(HOME)/bin/python2.6
CC               = gcc
CFLAGS           = -std=c99 $(shell $(PYTHON)-config --cflags)
#LD_LIBRARY_PATH  = $(HOME)/lib
#export LD_LIBRARY_PATH
LDFLAGS          = -L$(LD_LIBRARY_PATH) $(shell $(PYTHON)-config --ldflags)
PYTHONPATH      +=.:termkey
export PYTHONPATH


all: clean build run_memleak run_tests

clean:
	@rm -rf build
	@rm -f ./test_memleak
	@find . -name '_termkey*so' -exec rm "{}" \;
	@find . -name '*.pyc' -exec rm "{}" \;
	@echo "Cleaned."

build_ext: clean
	$(PYTHON) setup.py build_ext -fi
	#mv termkey/termkey/_termkey.so termkey/

build_memleak: clean
	$(CC) -o test_memleak test_memleak.c $(CFLAGS) $(LDFLAGS)

build: build_ext build_memleak

run_memleak: build_ext build_memleak
	./test_memleak
	#LD_LIBRARY_PATH= gdb

grind_memleak: build_ext build_memleak
	valgrind --tool=memcheck --leak-check=full --show-reachable=yes --track-origins=yes \
		./test_memleak

run_tests: build_ext
	$(PYTHON) ~/bin/nosetests $* || $(PYTHON)

run_stress: build_ext
	$(PYTHON) test_memleak.py
