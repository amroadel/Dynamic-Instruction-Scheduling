#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "fake_ROB.h"

using namespace std;

typedef enum {
	IF, // Fetch
	ID, // Dispatch
	IS, // Issue
	EX, // Excute
	WB  // Writeback
} states;

struct state_info {
	int cycle;
	int duration;
};
struct instruction {
	long int PC;
	int op;
	int rd;
	int rs1, rs2;
	int tag;
	states state;
	state_info info[5];
};

int reg_file[128];

void FakeRetire();
void Execute();
void Issue();
void Dispatch();
void Fetch();
void Advance_Cycle();
void Printe_Instruction(instruction *);

int main(int argc, char *argv[])
{
	if (argc != 4) {
		cerr << "Usage: " << argv[0] << "<S> <N> <tracefile>" << endl;
		return 1;
	}
	if (stoi(argv[1]) < 1 || stoi(argv[2]) < 1) {
		cerr << "<S> and <N> must be numbers > 0" << endl;
		return 1;
	}
	int S = stoi(argv[1]);
	int N = stoi(argv[2]);
	ifstream tracefile(argv[3]);
	if (!tracefile.is_open()) {
		cerr << "Cannot open file" << endl;
		return 1;
	}

	return 0;
}