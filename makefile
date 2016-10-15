14:
	g++ -std=c++14 -O2 main.cpp -pthread && ./a.out
11:
	g++ -std=c++11 -O2 main.cpp -pthread && ./a.out
all:
	g++ -std=c++14 -O2 -pthread *cpp ./class/*.cpp && ./a.out
