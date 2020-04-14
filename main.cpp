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
void Dispatch(queue<instruction*> &dispatch_list,queue<instruction*> &issue_list, int);
void Fetch(ifstream & , fake_ROB<instruction> &, queue<instruction*> &dispatch_list, int &, int &);
void Execute(queue<instruction *> &execute_list, queue<instruction *> &issue_list);
void Issue(queue<instruction *> &issue_list, queue<instruction *> &execute_list);
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

void Dispatch(queue<instruction*> &dispatch_list,queue<instruction*> &issue_list, int S)
{
     
    instruction inst;
    for (int i = 0; i < dispatch_list.size(); i++)
    {
        inst = *dispatch_list.front();
        
        
        if(issue_list.size() <= S)
        {
            if (inst.state == ID)
            {
                dispatch_list.pop();
                inst.state = IS;
                //TODO: rename rs and rd
                if (reg_file[inst.rs1] == -1)
                    inst.ready1 = true;
                else
                    inst.ready1 = false;

                if (reg_file[inst.rs2] == -1)
                    inst.ready2 = true;
                else
                    inst.ready2 = false;
                
                reg_file[inst.rd] = inst.tag;
                issue_list.push(&inst);
            }
        }
        else 
            inst.info[ID].duration++; 

        
        if (inst.state == IF)
        {
            inst.state = ID; 
            dispatch_list.pop();
            dispatch_list.push(&inst);
        }    
        
    }
}

void Fetch (ifstream &tracefile , fake_ROB<instruction> &ROB, queue<instruction*> &dispatch_list, int &tag, int &fetch_bandwidth)
{
    instruction inst; 
    //do the fetch bandwidth comaprison in the main
    Parser(tracefile, inst);
    inst.state = IF;
    inst.tag = tag; 
    ROB.enque(inst);
    dispatch_list.push(&inst);
    fetch_bandwidth++;
    tag++; 
    
}