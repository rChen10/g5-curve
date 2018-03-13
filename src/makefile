all: line pixel screen matrix

pixel: include/pixel.h
	g++ -c src/pixel.cpp -o build/pixel.o

line: include/line.h
	g++ -c src/line.cpp -o build/line.o

screen: include/screen.h
	g++ -c src/screen.cpp -o build/screen.o

matrix: include/matrix.h
	g++ -c src/matrix.cpp -o build/matrix.o

clean:
	rm build/* -r
	rm bin/* -r

