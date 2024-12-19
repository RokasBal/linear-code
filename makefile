SRC = $(wildcard */*.cpp) linear-code.cpp

build:
	g++ -O3 $(SRC) -o linear-code -fopenmp -march=native

run:
	make cleanLin
	make build
	.\linear-code.exe

debug:
	g++ -g $(SRC) -o linear-code-debug

clean:
	del *.o
	del linear-code.exe

cleanLin:
	rm -rf *.o
	rm -rf linear-code