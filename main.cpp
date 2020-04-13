#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct instruction {
	long int PC;
	int op;
	int rd;
	int rs1, rs2;
};

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