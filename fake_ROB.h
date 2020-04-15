#pragma once 
#include <iostream>
using namespace std;
template <class T>
class fake_ROB
{
private:
    /* data */
    int size;
     
public:
    T *array;
    int rear, front;
    fake_ROB();
    fake_ROB(int s);
    void enque (T inst);
    T  deque();
    int get_size ();  
    void display ();

};


