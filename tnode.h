#ifndef TNODE_H
#define TNODE_H


//node class for tree
template <typename T>
class tNode
{
public:
    tNode();
    tNode(T &d, int c = 1, int dc = 0);
    ~tNode();
    tNode<T>& operator=(const tNode<T> &b);
    //public accessors
    int& setDep_comp();
    int& setCount();
    T& setData();

    //overloaded comparison operators
    template <typename U>
    friend
    bool operator<(const tNode<U> &a, const tNode<U> &b);

    template <typename U>
    friend
    bool operator<=(const tNode<U> &a, const tNode<U> &b);

    template <typename U>
    friend
    bool operator>(const tNode<U> &a, const tNode<U> &b);

    template <typename U>
    friend
    bool operator>=(const tNode<U> &a, const tNode<U> &b);

    template <typename U>
    friend
    bool operator==(const tNode<U> &a, const tNode<U> &b);

    template <typename U>
    friend
    bool operator!=(const tNode<U> &a, const tNode<U> &b);

private:
    int count, dep_comp;
    T data;

    template <typename U>
    friend class bTree;
};

template <typename T>
tNode<T>::tNode()
{
    count = dep_comp = 0;
}

template <typename T>
tNode<T>::tNode(T &d, int c, int dc)
{
    data = d;
    count = c;
    dep_comp = dc;
}

template <typename T>
tNode<T>::~tNode()
{
}

template <typename T>
tNode<T>& tNode<T>::operator =(const tNode<T> &b)
{
    data = b.data;
    count = b.count;
    dep_comp = b.dep_comp;
    return *this;
}

template <typename T>
int& tNode<T>::setDep_comp()
{
    return dep_comp;
}

template <typename T>
int& tNode<T>::setCount()
{
    return count;
}

template <typename T>
T& tNode<T>::setData()
{
    return data;
}

template <typename U>
bool operator<(const tNode<U> &a, const tNode<U> &b)
{
    return a.data < b.data;
}

template <typename U>
bool operator<=(const tNode<U> &a, const tNode<U> &b)
{
    return a.data <= b.data;
}

template <typename U>
bool operator>(const tNode<U> &a, const tNode<U> &b)
{
    return a.data > b.data;
}

template <typename U>
bool operator>=(const tNode<U> &a, const tNode<U> &b)
{
    return a.data >= b.data;
}

template <typename U>
bool operator==(const tNode<U> &a, const tNode<U> &b)
{
    return a.data == b.data;
}

template <typename U>
bool operator!=(const tNode<U> &a, const tNode<U> &b)
{
    return a.data != b.data;
}


#endif // TNODE_H
