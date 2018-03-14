#ifndef HEAP_H
#define HEAP_H

#include "Node.h"
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

enum HEAP_ERRORS {HEAP_EMPTY}; //Error for signifying when the heap is empty
enum HEAPTYPE {MINHEAP,MAXHEAP}; //The type of the heap--either a
//min heap or a max heap

template <typename T>
class Heap
{
public:
    //Member functions
    Heap(); //Default constructor
    Heap(T& data); //Constructor that initializes the heap with data
    Heap(const Heap& other); //Copy constructor
    ~Heap(); //Destructor
    void insert(T& data); //Insert data into the heap
    T& remove(); //Remove the smallest data from the heap if the
    //heap is a min heap and the largest if the heap is a max heap
    const T& peek(); //Peeks at the "top" data in the heap
    void clear(); //Remove all data from the heap
    int count(); //Returns the number of data in the heap
    bool isEmpty(); //Returns true if the heap is empty. Otherwise, returns
    //false
    int depth(); //Returns the depth of the heap
    Heap<T>& operator<<(T data); //Insert data into the heap
    Heap<T>& operator>>(T& data); //Remove data from the heap
    HEAPTYPE get_heaptype(); //Returns either MINHEAP or MAXHEAP
    //depending on whether the heap is currently a min or max heap
    void change_heaptype(); //If the heap is a min heap, changes
    //it to a max heap. If the heap is a max heap, changes it to a min heap.

private:
    //Private member functions
    //Functions for reheapifying the heap after insertion or removal
    //of data follow
    void reheapify_up_min();
    void reheapify_up_max();
    void reheapify_down_min();
    void reheapify_down_max();
    void swap(int index1, int index2); //Swaps the values of the
    //Node pointers v[index1] and v[index2]
    double log2(double x); //Returns the log base 2 of x
    void nukem(); //Clears the heap
    void purge(int index); //Deallocates the subtree with
    //v[index] as the pointer to its root node
    void copy(const Heap& other); //Makes the calling Heap object
    //a copy of other

    //Private member variables
    vector<Node<T>*> v; //Vector of Node pointers where the pointers
    //point to nodes containing relevant data
    int lastIndex; //The index of the pointer in v that points to
    //the last node in the heap. This index is 0 when the heap is empty.
    static const int ROOT; //Shared constant that denotes the index
    //of v at which the pointer to the root node of the heap is stored
    HEAPTYPE ht; //Keeps track of the current heap type of the
    //heap--either MINHEAP or MAXHEAP depending on whether the heap is
    //a  min heap or max heap, respectively
};

template<typename T> 
const int Heap<T>::ROOT = 1; //Initializes the index of v at which
//the pointer to the root node of the heap is stored to 1

template<typename T>
Heap<T>::Heap()
{
    Node<T>* dummy = NULL;
    v.insert(v.end(),1,dummy);
    lastIndex = 0;
    ht = MAXHEAP;
}

template<typename T>
Heap<T>::Heap(T& data)
{
    Node<T>* hold = NULL;
    v.push_back(hold);
    hold = new Node<T>(data);
    v.push_back(hold);
    lastIndex = ROOT;
    ht = MAXHEAP;
}

template<typename T>
Heap<T>::Heap(const Heap& other)
{
    copy(other);
}

template<typename T>
Heap<T>::~Heap()
{
    purge(ROOT);
}

template<typename T>
void Heap<T>::insert(T& data)
{
    lastIndex++;
    if (lastIndex == v.size()) //Resize the vector to allow for a new row in
        //the heap if needed
    {
        Node<T>* dummy = NULL;
        v.insert(v.end(),v.size(),dummy);
    }
    v[lastIndex] = new Node<T>(data);
    if (ht)
    {
        reheapify_up_max();
    }
    else
    {
        reheapify_up_min();
    }
}

template<typename T>
T& Heap<T>::remove()
{
    if (lastIndex) //If there is a root
    {
        T* hold = (*(v[ROOT])).data; //Get a hold of the data to be returned
        (*(v[ROOT])).data = NULL;
        delete v[ROOT];
        v[ROOT] = v[lastIndex];
        v[lastIndex] = NULL;
        if (v[ROOT]) //If there's any nodes remaining after removal of the root
        {
            if (ht)
            {
                reheapify_down_max();
            }
            else
            {
                reheapify_down_min();
            }
        }
        if (lastIndex == v.size())//Resize the vector by eliminating a row in the
            //heap
        {
            v.erase(v.begin()+lastIndex,v.end());
        }
        lastIndex--;
        return *hold;
    }
    else
    {
        throw HEAP_EMPTY;
    }
}

template<typename T>
const T& Heap<T>::peek()
{
      if (lastIndex)
      {
       T* hold = (*(v[ROOT])).data;
       return *hold;
      }
      else
      {
          throw HEAP_EMPTY;
      }
}

template<typename T>
void Heap<T>::clear()
{
    nukem();
}

template<typename T>
int Heap<T>::count()
{
    return lastIndex;
}

template<typename T>
bool Heap<T>::isEmpty()
{
     return !lastIndex;
}

template<typename T>
int Heap<T>::depth()
{
    return (int)(log2(v.size()));
}

template<typename T>
Heap<T>& Heap<T>::operator<<(T data)
{
    insert(data);
    return *this;
}

template<typename T>
Heap<T>& Heap<T>::operator>>(T& data)
{
    try
    {
        data = remove();
    }
    catch(HEAP_ERRORS e)
    {
        if (e == HEAP_EMPTY)
        {
            cout<<"\nHeap is empty!\n";
        }
    }
    return *this;
}

template<typename T>
HEAPTYPE Heap<T>::get_heaptype()
{
    return ht;
}

template<typename T>
void Heap<T>::change_heaptype()
{
    nukem();
    ht?ht=MINHEAP:ht=MAXHEAP;
}

template<typename T>
void Heap<T>::reheapify_up_max()
{
    int index = lastIndex;
    while(index != ROOT && *v[index/2] < *v[index]) //Perform swapping with the parent
        //if necessary
    {
        swap(index,index/2);
        index = index/2;
    }
}

template<typename T>
void Heap<T>::reheapify_up_min()
{
    int index = lastIndex;
    while(index != ROOT && *v[index/2] > *v[index]) //Perform swapping with the parent
        //if necessary
    {
        swap(index,index/2);
        index = index/2;
    }
}

template<typename T>
void Heap<T>::reheapify_down_max()
{
    int index = ROOT;
    int limit = v.size();
    while((index*2 < limit) && (v[index*2] || v[index*2+1])) //Perform swapping
        //with children if necessary
    {
        if (v[index*2] && v[index*2+1]) //The examined node has two children
        {
            if ((*v[index] < *v[index*2]) || (*v[index] < *v[index*2+1]))
                //If the examined node is less than at least one of its children
            {
                if (*v[index*2] > *v[index*2+1])
                    //If the left child is greater than the right child
                {
                    swap(index,index*2);
                    index = index*2;
                }
                else //If the right child is greater than the left child
                {
                    swap(index,index*2+1);
                    index = index*2+1;
                }
            }
            else //The examined node is in the right place. Stop swapping.
            {
                break;
            }
        }
        else if (v[index*2]) //Only has left child
        {
            if (*v[index] < *v[index*2]) //Swap with child if child is greater
            {
                swap(index,index*2);
                index = index*2;
            }
            else //Stop swapping
            {
                break;
            }
        }
        else if (v[index*2+1]) //Only has right child
        {
            if (*v[index] < *v[index*2+1]) //Swap with child if child is greater
            {
                swap(index,index*2+1);
                index = index*2+1;
            }
            else
            {
                break; //Stop swapping
            }
        }
    }
}

template<typename T>
void Heap<T>::reheapify_down_min()
{
    int index = ROOT;
    int limit = v.size();
    while((index*2 < limit) && (v[index*2] || v[index*2+1])) //Perform swapping
        //with children if necessary
    {
        if (v[index*2] && v[index*2+1]) //The examined node has two children
        {
            if ((*v[index] > *v[index*2]) || (*v[index] > *v[index*2+1]))
                //If the examined node is greater than at least one of its children
            {
                if (*v[index*2] < *v[index*2+1])
                    //If the left child is less than the right child
                {
                    swap(index,index*2);
                    index = index*2;
                }
                else //If the right child is less than the left child
                {
                    swap(index,index*2+1);
                    index = index*2+1;
                }
            }
            else //The examined node is in the right place. Stop swapping.
            {
                break;
            }
        }
        else if (v[index*2]) //Only has left child
        {
            if (*v[index] > *v[index*2]) //Swap with child if child is lesser
            {
                swap(index,index*2);
                index = index*2;
            }
            else //Stop swapping
            {
                break;
            }
        }
        else if (v[index*2+1]) //Only has right child
        {
            if (*v[index] > *v[index*2+1]) //Swap with child if child is lesser
            {
                swap(index,index*2+1);
                index = index*2+1;
            }
            else
            {
                break; //Stop swapping
            }
        }
    }
}

template<typename T>
void Heap<T>::swap(int index1, int index2)
{
    Node<T>* hold = v[index1];
    v[index1] = v[index2];
    v[index2] = hold;
}

template<typename T>
double Heap<T>::log2(double x)
{
    return log10(x)/log10(2);
}

template<typename T>
void Heap<T>::nukem()
{
    purge(ROOT);
    v.erase(v.begin()+ROOT,v.end()); //Deallocate all vector elements except the first
}

template<typename T>
void Heap<T>::purge(int index)
{
    if (index < v.size() && v[index])
        //If the node to be examined exists...
    {
        delete v[index];
        v[index] = NULL;
        purge(index*2);
        purge(index*2+1);
    }
}

template<typename T>
void Heap<T>::copy(const Heap& other)
{
    nukem();
    Node<T>* hold;
    for (int i = 0; i < other.v.size(); i++)
    {
        hold = new Node<T>(*(other.v[i]));
        v.push_back(hold);
    }
    lastIndex = other.lastIndex;
    ht = other.ht;
}

#endif //HEAP_H
