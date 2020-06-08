build:
	rm -f program.exe
	g++ -O2 -std=c++11 -Wall main.cpp -o program.exe
	
run:
	./program.exe