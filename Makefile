all: main.o fake_ROB.o
	g++ -ggdb main.o fake_ROB.o -o sim

main.o: main.cpp
	g++ -c -ggdb main.cpp

fake_ROB.o: fake_ROB.cpp fake_ROB.h
	g++ -c -ggdb fake_ROB.cpp

.PHONY: clean
clean:
	rm -f main.o fake_ROB.o sim