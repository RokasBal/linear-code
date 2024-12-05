build: linear-code.o generateMatrix.o
	g++ linear-code.o generateMatrix.o -o linear-code

run:
	make cleanLin
	make build
	.\linear-code.exe

debug: generateMatrix.o
	g++ -g linear-code.cpp generateMatrix.o

linear-code.o: linear-code.cpp
	g++ -c linear-code.cpp

generateMatrix.o:
	cd utils
	g++ -c utils/generateMatrix.cpp

clean:
	del *.o
	del linear-code.exe

cleanLin:
	rm -rf *.o
	rm -rf linear-code