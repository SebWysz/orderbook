.PHONY: debug build test run clean

debug:
	cmake --preset debug

build:
	cmake --build --preset debug

test:
	ctest --preset test-debug

run:
	./build/debug/lob_main

clean:
	rm -rf build
