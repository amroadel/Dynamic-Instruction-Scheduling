#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
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
	bool ready1, ready2;
	int tag;
	states state;
	state_info info[5];
};

int reg_file[128];

void FakeRetire(fake_ROB<instruction> &);
void Execute(queue<instruction *> &execute_list, queue<instruction *> &issue_list);
void Issue(queue<instruction *> &issue_list, queue<instruction *> &execute_list);
void Dispatch();
void Fetch(ifstream & , fake_ROB<instruction> &, queue<instruction> dispatch_list, int);
void Advance_Cycle();
void Printe_Instruction(instruction *);
void Parser (ifstream &, instruction &);

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

//read instruction from the trace 
void Parser (ifstream &tracefile, instruction &inst)
{
    string PC, op, rd , rs1, rs2;
    tracefile>>PC>>op>>rd>>rs1>>rs2;
    inst.PC = stol(PC);
    inst.op = stoi(op);
    inst.rd = stoi(rd);
    inst.rs1 = stoi(rs1);
    inst.rs2 = stoi(rs2);
}

void FakeRetire(fake_ROB<instruction> &ROB)
{
    if (ROB.get_size() == 0)
    {
        cout<< "\nROB is empty";
        return;
    }
    instruction inst;
    do
    {
        if (ROB.array[ROB.front].state == WB)
        {
            inst = ROB.deque();
            //printinng function here  
        }
        else return;
        
    } while (ROB.get_size() != 0);
    
}

void Execute(queue<instruction *> &execute_list, queue<instruction *> &issue_list)
{
	for (int i = 0; i < execute_list.size(); i++) {
		instruction *temp = execute_list.front();
		execute_list.pop();
		if (temp->state == EX) {
			if ((temp->op == 0 && temp->info[temp->state].duration == 1)
				|| (temp->op == 1 && temp->info[temp->state].duration == 2)
				|| (temp->op == 2 && temp->info[temp->state].duration == 5)) {
				temp->state = WB;
				reg_file[temp->rd] = -1;
			} else {
				temp->info[temp->state].duration++;
				execute_list.push(temp);
			}
		} else {
			execute_list.push(temp);
		}
	}
	
	for (int i = 0; i < issue_list.size(); i++) {
		instruction *temp = issue_list.front();
		issue_list.pop();
		temp->ready1 = reg_file[temp->rs1] == -1;
		temp->ready2 = reg_file[temp->rs2] == -1;
		issue_list.push(temp);
	}
}

void Issue(queue<instruction *> &issue_list, queue<instruction *> &execute_list, int N)
{
	int issued = 0;
	for (int i = 0; i < issue_list.size(); i++) {
		instruction *temp = issue_list.front();
		issue_list.pop();
		if (temp->ready1 && temp->ready2) {
			temp->state = EX;
			execute_list.push(temp);
			if(N = issued++)
				break;
		} else {
			issue_list.push(temp);
		}
	}
}

void Fetch (ifstream &tracefile , fake_ROB<instruction> &, queue<instruction> dispatch_list, int N)
{
    int fetch_bandwidth = 0; //need to know what this is
    instruction inst; 
    while(!tracefile.eof())
    {
        if (!(dispatch_list.size() > 2*N) && fetch_bandwidth <= N)
        {
            Parser(tracefile, inst);
        }
    }

}