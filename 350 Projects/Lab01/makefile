prog: numbers.o 
	g++ numbers.o 

numbers.o: numbers.cpp 
	g++ -g -Wall -std=c++11 -c numbers.cpp

clean:
	rm -f prog *.o

rebuild:
	make clean
	make
