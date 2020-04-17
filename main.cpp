#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include "fake_ROB.cpp"
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
void Dispatch(queue<instruction*> &dispatch_list,queue<instruction*> &issue_list, int S, int cycles, int &fetch_bandwidth);
void Fetch(ifstream & , fake_ROB<instruction> &, queue<instruction*> &dispatch_list, int N, int cycles, int &tag, int &fetch_bandwidth);
void Execute(queue<instruction *> &execute_list, queue<instruction *> &issue_list, int cycles);
void Issue(queue<instruction *> &issue_list, queue<instruction *> &execute_list, int N, int cycles);
bool Advance_Cycle(ifstream &tracefile , fake_ROB<instruction> &ROB, int &cycles);
void Print_Instruction(instruction &);
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

	queue<instruction*> dispatch_list;
	queue<instruction*> issue_list;
	queue<instruction*> execute_list;
	fake_ROB<instruction> fifo(1024);
	int tag = 0;
	int fetch_bandwidth = 0;
	int cycles = 0;

	for (int i = 0; i < 128; i++)
		reg_file[i] = -1;
	
	do
	{
		//cout << cycles << " shit" << endl; // TODO: This should get removed
		FakeRetire(fifo);
		Execute(execute_list, issue_list, cycles);
		Issue(issue_list, execute_list, N, cycles);
		Dispatch(dispatch_list,issue_list, S, cycles, fetch_bandwidth);
		Fetch(tracefile, fifo, dispatch_list, N, cycles, tag, fetch_bandwidth);
	} while (Advance_Cycle(tracefile , fifo, cycles));

	return 0;
}

//read instruction from the trace 
void Parser (ifstream &tracefile, instruction &inst)
{
    string PC, op, rd , rs1, rs2;
    tracefile >> PC >> op >> rd >> rs1 >> rs2;
	if (PC == "")
		return;
    inst.PC = stol(PC); // TODO: make it read from hex
    inst.op = stoi(op);
    inst.rd = stoi(rd);
    inst.rs1 = stoi(rs1);
    inst.rs2 = stoi(rs2);
	inst.ready1 = (inst.rs1 == -1) ? true : false;
	inst.ready2 = (inst.rs2 == -1) ? true : false;
	for (int i = 0; i < 5; i++) {
		inst.info[i].cycle = 0;
		inst.info[i].duration = 0;
	}
}

void FakeRetire(fake_ROB<instruction> &ROB)
{
    if (ROB.get_size() == 0)
    {
        //cout<< "ROB is empty" << endl; // TODO: This should get removed
        return;
    }
    instruction inst;
    do
    {
        if (ROB.array[ROB.front].state == WB)
        {
            inst = ROB.deque();
			//cout << "shit inside FakeRetire" << endl; // TODO: This should get removed
            Print_Instruction(inst);
        }
        else return;
        
    } while (ROB.get_size() != 0);
    
}

void Execute(queue<instruction *> &execute_list, queue<instruction *> &issue_list, int cycles)
{
	int num = execute_list.size();
	for (int i = 0; i < num; i++) {
		instruction *temp = execute_list.front();
		execute_list.pop();
		if ((temp->op == 0 && temp->info[temp->state].duration == 1)
			|| (temp->op == 1 && temp->info[temp->state].duration == 2)
			|| (temp->op == 2 && temp->info[temp->state].duration == 5)) {
			temp->state = WB;
			temp->info[WB].cycle = cycles;
			temp->info[WB].duration = 1;
			if (temp->rd != -1)
				reg_file[temp->rd] = -1;
		} else {
			temp->info[EX].duration++;
			execute_list.push(temp);
		}
	}
	
	num = issue_list.size();
	for (int i = 0; i < num; i++) {
		instruction *temp = issue_list.front();
		issue_list.pop();
		temp->ready1 = (temp->ready1) ? true : reg_file[temp->rs1] == -1;
		temp->ready2 = (temp->ready2) ? true : reg_file[temp->rs2] == -1;
		issue_list.push(temp);
	}
}

void Issue(queue<instruction *> &issue_list, queue<instruction *> &execute_list, int N, int cycles)
{
	int issued = 0;
	int num = issue_list.size();
	for (int i = 0; i < num; i++) {
		instruction *temp = issue_list.front();
		issue_list.pop();
		if (temp->ready1 && temp->ready2) {
			temp->state = EX;
			temp->info[EX].cycle = cycles;
			temp->info[EX].duration = 1;
			execute_list.push(temp);
		} else {
			temp->info[IS].duration++;
			issue_list.push(temp);
		}
		issued++;
		if(issued == N + 1)
			break;
	}
}

void Dispatch(queue<instruction*> &dispatch_list,queue<instruction*> &issue_list, int S, int cycles, int &fetch_bandwidth)
{
     
    instruction *inst;
	int num = dispatch_list.size();
    for (int i = 0; i < num; i++)
    {
        inst = dispatch_list.front();
        
        
		if (inst->state == ID)
        {
        	if(issue_list.size() < S)
            {
                dispatch_list.pop();
                inst->state = IS;
				inst->info[IS].cycle = cycles;
				inst->info[IS].duration = 1;
                inst->ready1= (inst->ready1) ? true : (reg_file[inst->rs1] == -1) ? true : false;
                inst->ready2= (inst->ready2) ? true : (reg_file[inst->rs2] == -1) ? true : false;
                reg_file[inst->rd] = inst->tag;

                issue_list.push(inst);
            }
			else 
				inst->info[ID].duration++; 
        }

        
        if (inst->state == IF)
        {
            inst->state = ID;
			inst->info[ID].cycle = cycles;
			inst->info[ID].duration = 1;
            dispatch_list.pop();
            dispatch_list.push(inst);
			fetch_bandwidth--;
        }    
        
    }
}

void Fetch (ifstream &tracefile , fake_ROB<instruction> &ROB, queue<instruction*> &dispatch_list, int N, int cycles, int &tag, int &fetch_bandwidth)
{
	int fetched = 0;
    while (fetched < N && dispatch_list.size() <= 2 * N && !tracefile.eof()) {
    	instruction inst; 
		Parser(tracefile, inst);
		inst.state = IF;
		inst.info[IF].cycle = cycles;
		inst.info[IF].duration = 1;
		inst.tag = tag; 
		ROB.enque(inst);
		dispatch_list.push(&ROB.array[ROB.rear]);
		fetched++;
		tag++; 
	}
}

void Print_Instruction(instruction &inst){
	cout << inst.tag << " fu{" << inst.op << "} src{" << inst.rs1 << "," << inst.rs2 << "} dst{" << inst.rd << "} "
	<< "IF{" << inst.info[0].cycle << "," << inst.info[0].duration << "} "
	<< "ID{" << inst.info[1].cycle << "," << inst.info[1].duration << "} "
	<< "IS{" << inst.info[2].cycle << "," << inst.info[2].duration << "} "
	<< "EX{" << inst.info[3].cycle << "," << inst.info[3].duration << "} "
	<< "WB{" << inst.info[4].cycle << "," << inst.info[4].duration << "}" << endl;
}

bool Advance_Cycle(ifstream &tracefile , fake_ROB<instruction> &ROB, int &cycles)
{
	cycles++;
	if (ROB.get_size() == 0 && tracefile.eof())
		return false;
	else
		 return true;
}