golay.exe: main.cpp golay.o
	g++ -std=c++17 -o golay.exe main.cpp golay.o

golay.o: golay.cpp golay.h
	g++ -c -std=c++17 golay.cpp