#ifndef NODE_H
#define NODE_H

#include <iostream>

using namespace std;

enum NODE_ERR{NODE_COMP}; //Node errors. NODE_COMP is thrown if there
//is an error during any node comparison

template<typename T>
class Node
{
public:
    //Member functions
    Node(); //Default constructor
    Node(T& newdata); //Constructor with the data and the
    //count of the data the node will be initialized with
    Node(const Node& other); //Copy constructor
    ~Node(); //Destructor

    //Overloaded comparison operators follow (the return values
    //are the return values of the analogous comparison on the contained
    //data)...
    template<typename U>
    friend
    bool operator==(const Node<U>& n1, const Node<U>& n2);

    template<typename U>
    friend
    bool operator>(const Node<U>& n1, const Node<U>& n2);

    template<typename U>
    friend
    bool operator<(const Node<U>& n1, const Node<U>& n2);

    template<typename U>
    friend
    bool operator>=(const Node<U>& n1, const Node<U>& n2);

    template<typename U>
    friend
    bool operator<=(const Node<U>& n1, const Node<U>& n2);

    template<typename U>
    friend
    ostream& operator<<(ostream& out, Node<U>& n); //Prints the node

    template<typename U>
    friend
    istream& operator>>(istream& in, Node<U>& n); //Inputs data into
    //the node

    //Member variables
    T* data; //Pointer to the relevant data
};


template<typename T>
Node<T>::Node()
{
    data = new T();
}

template<typename T>
Node<T>::Node(T& newdata)
{
    data = new T(newdata);
}

template<typename T>
Node<T>::Node(const Node& other)
{
    data = new T(*(other.data));
}

template<typename T>
Node<T>::~Node()
{
    if (data) //If data points to any data, deallocate the data pointed to
    {
        delete data;
        data = NULL;
    }
}

template<typename U>
bool operator==(const Node<U>& n1, const Node<U>& n2)
{
    if (n1.data && n2.data)
    {
        return *(n1.data) == *(n2.data);
    }
    else
    {
        throw NODE_COMP;
    }
}

template<typename U>
bool operator>(const Node<U>& n1, const Node<U>& n2)
{
    if (n1.data && n2.data)
    {
        return *(n1.data) > *(n2.data);
    }
    else
    {
        throw NODE_COMP;
    }
}

template<typename U>
bool operator<(const Node<U>& n1, const Node<U>& n2)
{
    if (n1.data && n2.data)
    {
        return *(n1.data) < *(n2.data);
    }
    else
    {
        throw NODE_COMP;
    }
}

template<typename U>
bool operator>=(const Node<U>& n1, const Node<U>& n2)
{
    if (n1.data && n2.data)
    {
        return *(n1.data) >= *(n2.data);
    }
    else
    {
        throw NODE_COMP;
    }
}

template<typename U>
bool operator<=(const Node<U>& n1, const Node<U>& n2)
{
    if (n1.data && n2.data)
    {
        return *(n1.data) <= *(n2.data);
    }
    else
    {
        throw NODE_COMP;
    }
}

template<typename U>
ostream& operator<<(ostream& out, Node<U>& n)
{
    out<<*(n.data);
    return out;
}

template<typename U>
istream& operator>>(istream& in, Node<U>& n)
{
    char junk;
    in>>*(n.data);
    return in;
}

#endif // NODE_H
