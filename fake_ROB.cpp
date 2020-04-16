#include "fake_ROB.h"

template <class T>
fake_ROB<T>::fake_ROB()
{   
    size = 1024;
    current_size = 0;
    front = -1;
    rear = -1;
    array = new T [1024];
}

template <class T>
fake_ROB<T>::fake_ROB(int s)
{   
    size = s;
    current_size = 0;
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
    current_size++;
}

//remove instruction from the front
template <class T>
T  fake_ROB<T>::deque()
{
    if (front != -1) 
    {
    T output = array[front];
    //array[front]= -1;
    if (rear == front) front = rear = -1;
    else if (front == size -1) front = 0; 
    else front++;
    current_size--;
    return output; 
    } 
   cout<<"\nROB is empty";
}

//size getter
template <class T>
int fake_ROB<T>::get_size ()
{
    return current_size;
}

template <class T>
void fake_ROB<T>::display ()
{
    if (front == -1)
    {
        cout<<"\nROB is empty";
        return;
    }
    cout<<endl<<"Elements are: ";
    if (rear >= front)
    {
        for (int i = front ; i <= rear; i++)
            cout<<array[i]<<" "<<endl;
    }
    else 
    {
        for (int i = front;  i < size ; i++)
            cout<<array[i]<<" "<<endl;
        for (int i = 0; i <= rear; i++) 
            cout<<array[i]<<" "<<endl;
    }
}