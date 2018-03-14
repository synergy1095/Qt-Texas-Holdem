#ifndef MVECTOR_H
#define MVECTOR_H

#include <cstdlib>
#include <cmath>
#include <QDebug>

using namespace std;

typedef unsigned int size_type;

enum V_ERRORS {BAD_ALLOC};

template <typename T>
class mVector
{
public:
    explicit mVector(); //constructor
    explicit mVector(size_type n, const T x = T()); //constructor that fills with x
    mVector(const mVector<T> &other); //copy constructor
    mVector<T>& operator=(const mVector<T> &other); //copy operator
    ~mVector(); //destructor

    int size(); //returns size of vector
    void resize(size_type c, T x = T()); //resizes vector
    int capacity(); //returns capactiy
    bool empty(); //returns !size
    void reserve(size_type c); //reserves capacity does not change size
    T& operator[](size_type n); //accessor operator
    const T& at(size_type n); //peek function
    T& front(); //returns front
    T& back(); //returns last
    void push_back(const T &x); //adds x to end of vector increasing size by one
    void pop_back(); //pop off the end of the vector decreasing size by one
    void clear(); //public boom function

private:
    T** list_; //an array of arrays
    size_type* list_s; //array holding the size of each array
    size_type def_size, list_size, list_qty;
    size_type vec_size, cap;

    void nukem(); //boom function
    void init(); //initializes list and list_s
    void res_fill(size_type &c, T &x); //fill function for resize
    void cleanup(size_type l_slot); //clean up any empty arrays
    size_type find_l_slot(const size_type &n); //returns the slot of array
    void find_l_slot(size_type &n, size_type &s); //sets passed in values to
                                                 //slot in array and slot of array
    void copy(const mVector<T> &other); //copy function
};
//constructor
template <typename T>
mVector<T>::mVector()
{
    list_ = NULL;
    list_s = NULL;
    list_qty = 0;
    list_size = 25;
    def_size = 10;
    vec_size = 0;
    cap = 0;
}
//constructor that fills with x
template <typename T>
mVector<T>::mVector(size_type n, const T x)
{
    list_ = NULL;
    list_s = NULL;
    list_qty = 0;
    list_size = 25;
    def_size = 10;
    vec_size = 0;
    cap = 0;
    resize(n, x);
}
//copy constructor
template <typename T>
mVector<T>::mVector(const mVector<T> &other)
{
    list_ = NULL;
    list_s = NULL;
    list_qty = 0;
    list_size = 25;
    def_size = 10;
    vec_size = 0;
    cap = 0;
    copy(other);
}
//boom function
template <typename T>
void mVector<T>::nukem()
{
    if(list_)//if there's something to delete
    {
        for(int i = 0; i < list_qty; i++)
        {
            if(list_[i])    //delete each array in slots
            {
                delete [] (list_[i]);
                list_[i] = NULL;
            }
        }
        delete [] list_; //then delete the container arrays
        delete [] list_s;
    }

    list_ = NULL;
    list_s = NULL;
    list_qty = 0;
    list_size = 25;
    def_size = 10;
    vec_size = 0;
    cap = 0;
}
//copy operator
template <typename T>
mVector<T> &mVector<T>::operator =(const mVector<T> &other)
{
    copy(other);
    return *this;
}
//destructor
template <typename T>
mVector<T>::~mVector()
{
    nukem();
}
//returns size of vector
template <typename T>
int mVector<T>::size()
{
    return vec_size;
}
//fills new slots for resize
template <typename T>
void mVector<T>::res_fill(size_type &c, T &x)
{

    size_type s_slot = vec_size, s_l_slot;
    size_type e_slot = c-1, e_l_slot;
    vec_size = c;

    find_l_slot(s_slot, s_l_slot); //translate start
    find_l_slot(e_slot, e_l_slot); //translate end
    for(size_type i = s_l_slot; i < e_l_slot; i++)
    {
        for(size_type j = s_slot; j < list_s[i]; j++)
        {
            (list_[i])[j] = x;
        }
        s_slot = 0;
    }
    for(size_type i = s_slot; i <= e_slot; i++)
        (list_[e_l_slot])[i] = x;

/*  //above code is slightly faster than this but this is simpler
    size_type i = vec_size; //this increases run time by anywhere from 60-100ms
    vec_size = c;           //for war and peace
    for(; c-i; i++)
        (*this)[i] = x;*/ //having to translate slows it down
}
//resize function
template <typename T>
void mVector<T>::resize(size_type c, T x)
{
    if(c > cap) //if c greater than cap
    {
        if(c-cap > def_size)
            reserve(c);
        else
        {
            def_size *= 2;
            reserve(cap + def_size);
        }
        res_fill(c, x); //fill with x
    }
    else if(c > vec_size) //if wasn't greater than cap
    {                     //but not smaller or
        res_fill(c, x);
    }
    else        //otherwise we're shrinking vector
    {
        if(!c)
        {
            nukem();
            return;
        }
        if(!(c-vec_size)) //if c = vec size
            return;
        vec_size = c;
        cleanup(find_l_slot(vec_size-1)); //clean up any empty arrays
    }
}
//clean up empty array from shrinking the vector
template <typename T>
void mVector<T>::cleanup(size_type l_slot)
{
    l_slot++;
    if(l_slot >= list_qty)
        return;
    for(size_type i = l_slot; i < list_qty; i++)
    {
        delete [] (list_[i]); //delete and set to null arrays after
        list_[i] = NULL;
        cap -= list_s[i];
        list_s[i] = 0;
    }
    list_qty = l_slot;
}
//return capacity
template <typename T>
int mVector<T>::capacity()
{
    return cap;
}
//return true if empty
template <typename T>
bool mVector<T>::empty()
{
    return !(vec_size);
}
//reserve capacity does not change size, does not shrink vector
//use resize to shrink vector
template <typename T>
void mVector<T>::reserve(size_type c)
{
    if(!(c-cap))
        return;
    else if(c > cap)
    {
        if(!list_)//if list not started
            init();
        else if(!(list_size - list_qty))//if list_ is full
        {
            list_size *= 2;
            T** l_temp = new T*[list_size];
            size_type* ls_temp = new size_type[list_size];

            for(int i = 0; i < list_qty; i++)//copy and delete
            {
                l_temp[i] = list_[i];
                ls_temp[i] = list_s[i];
            }
            delete [] list_;
            delete [] list_s;

            list_ = l_temp;     //set to new locations
            list_s = ls_temp;
        }
        list_[list_qty] = new T[c-cap];
        list_s[list_qty] = c-cap;
        cap = c;
        list_qty++;
    }
}
//translates n to slots accessor
template <typename T>
T &mVector<T>::operator [](size_type n)
{
    if(!list_ || (n >= vec_size))
        throw BAD_ALLOC;

    size_type trans = n;
    size_type l_slot = 0;
    for(; l_slot < list_qty; l_slot++)
    {
        if(trans >= list_s[l_slot] )
            trans -= list_s[l_slot];
        else
        {
            return (list_[l_slot])[trans];
        }
    }
    throw BAD_ALLOC;
}
//translates n to slots and peeks
template <typename T>
const T &mVector<T>::at(size_type n)
{
    if(!list_ || (n >= vec_size))
        throw BAD_ALLOC;

    size_type trans = n;
    size_type l_slot = 0;
    for(; l_slot < list_qty; l_slot++)
    {
        if(trans >= list_s[l_slot] )
            trans -= list_s[l_slot];
        else
        {
            return (list_[l_slot])[trans];
        }
    }
    throw BAD_ALLOC;
}
//returns the slot of list_
template <typename T>
size_type mVector<T>::find_l_slot(const size_type &n)
{
    if(!list_ || (n >= vec_size))
        throw BAD_ALLOC;

    size_type trans = n;
    size_type l_slot = 0;
    for(; l_slot < list_qty; l_slot++)
    {
        if(trans >= list_s[l_slot] )
            trans -= list_s[l_slot];
        else
            return l_slot;
    }
    throw BAD_ALLOC;
}
//sets n and s to list[s][n] slots
template <typename T>
void mVector<T>::find_l_slot(size_type &n, size_type &s)
{
    if(!list_s || (n >= vec_size))
        return;

    size_type trans = n;
    size_type l_slot = 0;
    for(; l_slot < list_qty; l_slot++)
    {
        if(trans >= list_s[l_slot] )
            trans -= list_s[l_slot];
        else
        {
            n = trans;
            s = l_slot;
            return;
        }
    }
    throw BAD_ALLOC;
}
//front value accessor
template <typename T>
T &mVector<T>::front()
{
    return (*this)[0];
}
//back value accessor
template <typename T>
T &mVector<T>::back()
{
    return (*this)[vec_size-1];
}
//adds x to end of vector increasing size by one
template <typename T>
void mVector<T>::push_back(const T &x)
{
    if(!list_)
    {
        init();
        def_size *= 2;
        list_[0] = new T[def_size];
        list_s[0] = def_size;
        vec_size = 1;
        cap = def_size;
        list_qty = 1;
        (list_[0])[0] = x;
    }
    else
    {
        if(!(cap - vec_size))//once size is equal to capacity
        {
            def_size *= 2;
            reserve(cap + def_size); //increase capacity by default
            vec_size++;
            (*this)[vec_size-1] = x; //translate and set value
        }
        else
        {
            vec_size++;
            (*this)[vec_size-1] = x;
        }
    }
}
//pop off the end of the vector decreasing size by one
template <typename T>
void mVector<T>::pop_back()
{
    vec_size--;
    if(!vec_size)
    {
        nukem();
        return;
    }
    cleanup(find_l_slot(vec_size-1));
}
//public boom function
template <typename T>
void mVector<T>::clear()
{
    nukem();
}
//initializes list and list_s
template <typename T>
void mVector<T>::init()
{
    list_size *= 2;
    list_ = new T*[list_size];
    list_s = new size_type[list_size];
}
//copy function
template <typename T>
void mVector<T>::copy(const mVector<T> &other)
{
    if(this != &other)
    {
        if(!(other.vec_size))
            return;

        if(list_)   //vector has stuff empty it
            nukem();

        list_qty = other.list_qty; //copy variables
        list_size = other.list_size;
        def_size = other.def_size;
        vec_size = other.vec_size;
        cap = other.cap;

        list_s = new size_type[list_size]; //initialize
        list_ = new T*[list_size];

        for(size_type i = 0; list_qty-i; i++)
            list_s[i] = other.list_s[i];

        size_type e_slot = vec_size-1, e_l_slot = 0;
        find_l_slot(e_slot, e_l_slot); //find end slots works similar to res_fill()
        for(size_type i = 0; e_l_slot-i; i++)
        {
            list_[i] = new T[list_s[i]];
            for(size_type j = 0; list_s[i]-j; j++)
                (list_[i])[j] = ((other.list_)[i])[j];
        }
        list_[e_slot] = new T[list_s[e_slot]];
        for(size_type i = 0; i <= e_slot; i++)
            (list_[e_l_slot])[i] = ((other.list_)[e_l_slot])[i];
    }
}

#endif // MVECTOR_H
