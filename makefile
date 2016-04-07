ass2:main.o ass2.o
	g++ ass2.o main.o -o ass2
main.o:main.cpp ass2.h
	g++ main.cpp -c
ass2.o:ass2.cpp ass2.h
	g++ ass2.cpp -c
clean:
	rm ass2.o main.o ass2
