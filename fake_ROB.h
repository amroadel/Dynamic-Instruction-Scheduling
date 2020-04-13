#pragma once 
#include <iostream>
template <class T>
class fake_ROB
{
private:
    /* data */
    int size;
    T *array;
    int rear, front; 
public:

    fake_ROB(int s);
    void enque (T inst);
    T  deque();
    int get_size ();  
    void display ();

};


