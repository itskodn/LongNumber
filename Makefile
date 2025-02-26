PRECISION ?= 100

default_target:
	cmake -S . -B build && cd build && make

main:
	cd build && ./main

pi:
	cd build && ./pi $(PRECISION)

test:
	cd build && ./test

clean:
	rm -rf build

.PHONY: default_target main pi test clean