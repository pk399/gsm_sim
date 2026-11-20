
build/plot.png: build/main scripts/simple_ber.py
	python scripts/simple_ber.py

build/main: src/main.c build
	gcc -o build/main -O2 -losmocore -losmocoding src/main.c

build:
	mkdir build

clean:
	rm -r build
