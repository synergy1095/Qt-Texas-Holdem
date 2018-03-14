#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <cstdlib>

using namespace std;

enum STACK_ERRORS{EMPTY,FULL};

template<typename T>
class Stack
{
public:
    Stack();   //Default constructor
    Stack(int newmax);  //Constructor
    ~Stack();  //Destructor
    Stack(const Stack<T>& other);      //Copy constructor
    Stack& operator=(const Stack<T>& other);  //Assignment operator

    void push(T& item);   //If the stack is not full, pushes item onto the
    //stack.
    T* pop();   //If the stack is not empty, pops the last pushed item off of
    //the stack and returns it
    const T& peek() const;    //If the stack is not empty,returns the last
    //pushed item by reference. This item may not be
    //modified.
    bool isEmpty() const; //Returns true if the stack is empty. Otherwise,
    //returns false.
    bool isFull() const;  //Returns true if the stack is full. Otherwise,
    //returns false.
    int sizeOf() const;   //Returns the number of items currently in the stack.
    void clear(); //Clears the stack.
    void resize(int new_size); //Resizes the stack. Does not account for
    //non-natural numbers.

    template<typename U>
    friend
    ostream& operator<<(ostream& out, Stack<U> &s); //Overloaded extraction
    //operator

    template<typename U>
    friend
    istream& operator>>(istream& in, Stack<U> &s);  //Overloaded insertion
    //operator

private:
    void copy(const Stack<T>& other);   //Helper function that copies the
    //contents of other into the calling
    //object.
    void nukem();   //Helper function that deallocates the dynamic array
    //pointed to by ptr and sets all private member
    //variables to appropriate values.
    T** ptr; //Pointer to a dynamic array holding data of type T*.
    int maxitems;   //The capacity of the stack
    int tos;    //The index of the current top of stack
    //(last item pushed into the stack). If there are no items,
    //its value should be -1.
};


template<typename T>
Stack<T>::Stack()
{
    ptr = new T*[52];
    tos = -1;
    maxitems = 52;
}

template<typename T>
Stack<T>::Stack(int newmax)
{
    ptr = new T*[newmax];
    tos = -1;
    maxitems = newmax;
}

template<typename T>
Stack<T>::~Stack()
{
    nukem();
}

template<typename T>
Stack<T>::Stack(const Stack<T>& other)
{
    copy(other);
}

template<typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& other)
{
    if (this != &other)
    {
        copy(other);
    }
    return *this;
}

template<typename T>
void Stack<T>::push(T& item)
{
    if (!isFull())
    {
        tos++;
        ptr[tos] = new T;
        *(ptr[tos]) = item;
    }
    else
    {
        throw FULL;
    }
}

template<typename T>
T* Stack<T>::pop()
{
    //If there's something in the stack, decrement tos and then return the last
    //pushed item by reference.
    if (!isEmpty())
    {
        T* hold = ptr[tos];
        ptr[tos] = NULL;
        tos--;
        return hold;
    }
    else
    {
        throw EMPTY;
    }
}

template<typename T>
const T& Stack<T>::peek() const
{
    if (!isEmpty())
    {
        return *(ptr[tos]);
    }
    else
    {
        throw EMPTY;
    }
}

template<typename T>
bool Stack<T>::isEmpty() const
{
    return tos == -1;
}

template<typename T>
bool Stack<T>::isFull() const
{
    return tos + 1 == maxitems;
}

template<typename T>
int Stack<T>::sizeOf() const
{
    return tos + 1;
}

template<typename T>
void Stack<T>::clear()
{
    for (int i = 0; i <= tos; i++)
    {
        if (ptr[i])
        {
            delete ptr[i];
            ptr[i] = NULL;
        }
    }
    tos = -1;
}

template<typename T>
void Stack<T>::resize(int new_size)
{
    T** hold = new T*[new_size];
    //If the max size of the new stack is larger or equal...
    if (new_size >= maxitems)
    {
        for (int i = 0; i <= tos; i++)
        {
            hold[i] = ptr[i];
            ptr[i] = NULL;
        }
    }
    else //If the max size of the new stack is smaller...
    {
        //If the number of current items in this stack is less than or
        //equal to the max size of the new stack
        if (new_size > tos)
        {
            for (int i = 0; i <= tos; i++)
            {
                hold[i] = ptr[i];
                ptr[i] = NULL;
            }
        }
        else
            //If the number of current items in this stack is greater than the
            //max size of the new stack
        {
            for (int i = 0; i < new_size; i++)
            {
                hold[i] = ptr[i];
                ptr[i] = NULL;
            }
            for (int i = new_size; i <= tos; i++)
            {
                delete ptr[i];
                ptr[i] = NULL;
            }
        }
        tos = new_size - 1;
    }
    delete [] ptr;
    ptr = hold;
    maxitems = new_size;
}

template<typename U>
ostream& operator<<(ostream& out, Stack<U> &s)
{
    for (int i = 0; i <= s.tos; i++)
    {
        out<<"["<<*(s.ptr[i])<<"]";
    }
    return out;
}

template<typename U>
istream& operator>>(istream& in, Stack<U> &s)
{
    char junk;
    U data;
    while (in>>junk>>data>>junk)
    {
        if (s.isFull())
        {
            s.resize(s.sizeOf()+1);     //Very inefficient
        }
        s.push(data);
    }
    return in;
}

template<typename T>
void Stack<T>::copy(const Stack<T>& other)
{
    nukem();
    ptr = new T*[other.maxitems];
    for (int i = 0; i <= other.tos; i++)
    {
        ptr[i] = new T(*(other.ptr[i]));
    }
    for (int i = other.tos + 1; i < other.maxitems; i++)
    {
        ptr[i] = NULL;
    }
    maxitems = other.maxitems;
    tos = other.tos;
}

template<typename T>
void Stack<T>::nukem()
{
    clear();
    delete [] ptr;
    //Set member variables to the appropriate values...
    ptr = NULL;
}

#endif
