#include "fake_ROB.h"

template <class T>
fake_ROB<T>::fake_ROB(int s)
{   
    size = s;
    front = -1;
    rear = -1;
    array = new T [s];
}

//insert in the ROB
template <class T>
void fake_ROB<T>::enque (T inst)
{
    if ((rear == size-1 && front == 0) || (rear == (front-1)%(size-1))) 
    { 
        cout<<"\nFull ROB"; 
        return; 
    } 
    else if (front == -1) //Insert First instruction
    {
        front = 0;
        rear = 0;
        array [rear] = inst; 
    }
    else if (rear == size-1 && front != 0) 
    { 
        rear = 0; 
        array[rear] = inst; 
    } 
    else
    { 
        rear++; 
        array[rear] = inst; 
    } 
}

//remove instruction from the front
template <class T>
T  fake_ROB<T>::deque()
{
    if (front == -1) 
    {
        cout<<"\nROB is empty";
        return;
    } 
    T output = array[front];
    array[front]= -1;
    if (rear = fornt) front = rear = -1;
    else if (front = size -1) front = 0; 
    else front++; 
    return output;
}

//size getter
template <class T>
int fake_ROB<T>::get_size ()
{
    return size;
}

template <class T>
void fake_ROB<T>::display ()
{
    if (front == -1)
    {
        cout<<"\nROB is empty";
        return;
    }

    if (rear >= front)
    {
        for (int i = front ; i <rear; i++)
            cout<<array[i];
    }
    else 
    {
        for (int i = front  i < size ; i++)
            cout<<array[i];
        for (int i = 0; i <= rear; i++) 
            cout<<array[i];
    }
}