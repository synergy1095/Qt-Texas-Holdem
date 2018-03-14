#ifndef BTREE_H
#define BTREE_H

#include <QDebug>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "mvector.h"
#include <stack>
#include <queue>
#include "tnode.h"

using namespace std;

enum TRAVERSE {PRE, IN, POST};

//binary search tree with less side <- and greater side ->
template <typename T>
class bTree
{
public:
    bTree(); //constructor
    bTree(T &d, int c = 1); //constructor with initial values
    bTree(const bTree<T> &other); //copy constructor
    bTree<T>& operator=(const bTree<T> &other); //overloading copy operator
    ~bTree(); //destructor

    TRAVERSE& printOrder(); //order accessor

    T* insert(T &data, int c = 1); //insert function
    int find(T &data);               //find function returns count
    T* dAccess(T &data);             //data accessors
    T& Access(T &data);
    void getSortedData(mVector<T*> &save);//outputs tree data into
    void getSortedData(mVector<T> &save);//passed in mVector in current order
    bool remove(T &data, int c = 1);
    int clear();    //public nuke that returns number of nodes that went boom
    int depth();    //depth of tree
    int count();    //number of nodes in tree
    bool empty();

    void display();
    void print(ostream& out = cout);

    bTree<T>& operator<<(T &data);

    template<typename U>
    friend
    ostream& operator<<(ostream &out, bTree<U> &t);

    template<typename U>
    friend
    istream& operator>>(istream &in, bTree<U> &t);


private:
    void inOrder(int index);    //set order to hold slots in-order
    void preOrder(int index);   //set order to hold slots pre-order
    void postOrder(int index);  //set order to hold slots post-order
    void tSlot(int r, mVector<int> &v);//create a mVector holding the slots of tree from r
                                    //in format r, 2r+1, 2r+2, 2(2r+1)+1, 2(2r+2)+2, etc
    void tSlot_N(int r, mVector<tNode<T>*> &v);//remove subtree from r from tree and store
                                            //in passed in mVector
    void move_subTree(int to_r, mVector<tNode<T> *> &temp);//moves tree with data in tSlot
                                                    //format to position of to_r
    void rem_update(int index); //updates dep_comp for remove
    void rot_less(int index); //rotate to less side
    void rot_great(int index); //rotate to greater side
    void balance_ins(int index);    //balances tree from passed in index to root
    void balance_rem(int index);    //balances tree from passed in index to root

    void setOrder(int index);    //set order to contain slots in current order
    void setSize();  //reset tree size to have at least 1 empty tier under tree
    void nukem(int index);  //boom function
    void copy(const bTree<T> &other);   //copy passed in tree
    int depth(int r);   //depth of tree with root r
    int find(int r, T &d);    //find node
    int findParent(int r, T &d);  //find node parent even if node doesn't exist
    int count(int r);   //return size of subtree
    void print(ostream& out = cout, int r = 0);

    mVector<tNode<T>*> tree;
    queue<int> order;
    TRAVERSE trav;
    int num_node;
};

template <typename T>
bTree<T>::bTree() //constructor
{
    tree.resize(3, NULL);
    trav = IN;
    num_node = 0;
}

template <typename T>
bTree<T>::bTree(T &d, int c) //constructor with data
{
    tree.resize(3, NULL);
    insert(0, tNode<T>(d, c));
    num_node = 1;
    trav = IN;
}

template <typename T> //copy constructor
bTree<T>::bTree(const bTree<T> &other)
{
    tree.resize(3, NULL);
    trav = IN;
    num_node = 0;
    copy(other);
}

template <typename T>
bTree<T>& bTree<T>::operator=(const bTree<T>& other) //overloading copy operator
{
    if(this != &other)
    {
        nukem();
        copy(other);
    }
    return *this;
}

template <typename T>
bTree<T>::~bTree() //destructor
{
    nukem(0);
    tree.resize(0);
}

template <typename T>
TRAVERSE& bTree<T>::printOrder()  //traverse order accessor
{
    return trav;
}

template <typename T>
T *bTree<T>::insert(T &data, int c)  //insert data with count of c
{
    int par = findParent(0, data);
    int child;

    if(!(par+1)) //for root case if par = -1 this will be true
    {
        if(tree[0])
        {
            tree[0]->count += c;
        }
        else
        {
            tNode<T>* temp = new tNode<T>(data, c, 0);
            tree[0] = temp;
            num_node++;
        }
        return &(tree[0]->data);
    }
    //set which child and adjust dep_comp
    tree[par]->data > data ? child = 2*par+1 : child = 2*par+2;

    if(tree[child])
    {
        tree[child]->count += c;
        return &(tree[child]->data);
    }
    else
    {
        T* datatemp;
        tNode<T>* temp = new tNode<T>(data, c, 0);
        datatemp = &(temp->data);
        num_node++;
        if(2*child+3 > tree.size())
            tree.resize(2*child+3, NULL);

        tree[child] = temp;
        if(child%2)
        {
            tree[par]->dep_comp++;
        }
        else
        {
            tree[par]->dep_comp--;
        }

        int cur_par = par;
        while(cur_par && tree[cur_par]->dep_comp && (tree[cur_par]->dep_comp+2 && tree[cur_par]->dep_comp-2))
        {        //while not root and dep_comp isn't 0, -2, or 2 once we have a -2/2 we have a balance point
            if(cur_par%2) //no need to adjust past balance point since the rotation will just me we have to undo the adjustment
            {    //adjust depth comparison for parent and move up
                cur_par = (cur_par-1)/2;
                tree[cur_par]->dep_comp++;
            }
            else
            {
                cur_par = (cur_par-2)/2;
                tree[cur_par]->dep_comp--;
            }
        }
        balance_ins(par);
        return datatemp;
    }
}

//public find returns count
template <typename T>
int bTree<T>::find(T &data)
{
    int temp = find(0, data);

    return !(temp+1) ? 0 : tree[temp]->count;
}

//updates dep_comp for remove
template <typename T>
void bTree<T>::rem_update(int index)
{
    if(index <= 0)
        return;

    int cur;

    if(index%2) //initial setup
    {
        cur = (index-1)/2;
        tree[cur]->dep_comp--;
    }
    else
    {
        cur = (index-2)/2;
        tree[cur]->dep_comp++;
    }

    while(!(tree[cur]->dep_comp) && cur) //continue if dep_comp turns 0 and not root
    {
        if(cur%2)
        {
            cur = (cur-1)/2;
            tree[cur]->dep_comp--;
        }
        else
        {
            cur = (cur-2)/2;
            tree[cur]->dep_comp++;
        }
    }
}

template <typename T>
bool bTree<T>::remove(T &data, int c)  //remove data from try by c amount
{
    int rem = find(0, data);
    int par;

    rem%2 ? par = (rem-1)/2 : par = (rem-2)/2;

    if(rem > -1)    //if data is in tree
    {
        tree[rem]->setCount() -= c; //decrease count of data by c
        int l = 2*rem+1, g = 2*rem+2;
        if(tree[rem]->setCount() <= 0)//if count is now less than 0
        {           //no children case
            if(!(tree[l]) && !(tree[g]))
            {
                delete tree[rem];
                tree[rem] = NULL;

                num_node--;
                if(!num_node)
                    tree.resize(3, NULL);

                rem_update(rem);
                balance_rem(par);
            }
            else
            {
                if(!(tree[g])) //only less child exists case
                {
                    delete tree[rem];
                    tree[rem] = NULL;
                    num_node--;

                    mVector<tNode<T>*> save1;
                    tSlot_N(l, save1);
                    move_subTree(rem, save1);

                    rem_update(rem);
                    balance_rem(rem);
                }
                else if(!(tree[l])) //only greater child exists
                {
                    delete tree[rem];
                    tree[rem] = NULL;
                    num_node--;

                    mVector<tNode<T>*> save1;
                    tSlot_N(g, save1);
                    move_subTree(rem, save1);

                    rem_update(rem);
                    balance_rem(rem);
                }
                else        //both children exist
                {
                    int r_dep = tree[rem]->dep_comp;
                    delete tree[rem];
                    tree[rem] = NULL;
                    num_node--;

                    if(r_dep > 0)
                    {
                        int most_g = l;
                        for(; tree[2*most_g+2]; most_g = 2*most_g+2);

                        mVector<tNode<T>*> savegl; //save most_g less
                        tSlot_N(2*most_g+1, savegl);
                        mVector<tNode<T>*> saveg; //save most_g
                        tSlot_N(most_g , saveg);
                        mVector<tNode<T>*> save2; //store less subtree
                        tSlot_N(l, save2);
                        mVector<tNode<T>*> save3; //store greater subtree
                        tSlot_N(g, save3);

                        move_subTree(rem, saveg);
                        move_subTree(l, save2);
                        move_subTree(g, save3);
                        move_subTree(most_g, savegl);

                        tree[rem]->dep_comp = r_dep;
                        rem_update(most_g);
                        balance_rem((most_g-2)/2);
                    }
                    else
                    {
                        int most_l = g;
                        for(; tree[2*most_l+1]; most_l = 2*most_l+1);

                        mVector<tNode<T>*> savelg; //save most_l greater
                        tSlot_N(2*most_l+2, savelg);
                        mVector<tNode<T>*> savel; //save most_l
                        tSlot_N(most_l , savel);
                        mVector<tNode<T>*> save2; //store less subtree
                        tSlot_N(l, save2);
                        mVector<tNode<T>*> save3; //store greater subtree
                        tSlot_N(g, save3);

                        move_subTree(rem, savel);
                        move_subTree(l, save2);
                        move_subTree(g, save3);
                        move_subTree(most_l, savelg);

                        tree[rem]->dep_comp = r_dep;
                        rem_update(most_l);
                        balance_rem((most_l-1)/2);
                    }
                }
            }
        }
        setSize();
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T>
void bTree<T>::display()
{
    print(cout, 0);
}

template <typename T>
int bTree<T>::clear()  //public empty tree returns size of tree before disposal
{
    if(!num_node)
        return 0;
    int qty = num_node;
    nukem(0);
    return qty;
}

template <typename T>
int bTree<T>::depth() //returns depth of tree
{
    return depth(0);
}

template <typename T>
void bTree<T>::print(ostream& out)  //public print
{
    print(out, 0);
}

template <typename T>
void bTree<T>::print(ostream& out, int r)  //private print
{
    setOrder(r);

    while(!order.empty())
    {
        out << tree[order.front()]->data;
          //<< "(" << order.front() << "," << tree[order.front()]->dep_comp <<") ";
        order.pop();
    }
}

template <typename T>
bTree<T>& bTree<T>::operator<<(T &data)
{
    insert(data);
    return *this;
}

template <typename U>
ostream& operator<<(ostream &out, bTree<U> &t)
{
    t.print(out, 0);
    return out;
}

template <typename U>
istream& operator>>(istream &in, bTree<U> &t)
{
    U d;
    while(in>>d)
        t.insert(d);

    return in;
}

template <typename T>
void bTree<T>::rot_great(int index)
{
    int cur = index;
    int l = 2*cur+1, g = 2*cur+2;
    int gl = 2*g+1;
    int ll = 2*l+1, lg = 2*l+2;
    mVector<tNode<T>*> save1, save2, save3;
    //check if less greater heavy
    if(tree[l]->dep_comp < 0)
    {
        tSlot_N(2*lg+1, save1);//store less side of lg
        tSlot_N(lg, save2);//store less greater
        tSlot_N(l, save3);//store less

        move_subTree(l, save2);//move less greater to original less location
        move_subTree(ll, save3);//move less to less side of less greater
        move_subTree(2*ll+2, save1);//move less side of lg to greater side of less
        //should not need a situation with only greater child
        if(tree[2*ll+1] && !(tree[2*ll+2]))
            tree[ll]->dep_comp = 1;
        else
            tree[ll]->dep_comp = 0;
    }
    //less less case
    tSlot_N(lg, save1);
    tSlot_N(l, save2);
    tSlot_N(cur, save3);

    move_subTree(cur, save2);
    move_subTree(g, save3);
    move_subTree(gl, save1);

    if(tree[2*g+1] && !(tree[2*g+2]))
        tree[g]->dep_comp = 1;
    else
        tree[g]->dep_comp = 0;

    tree[index]->dep_comp = 0;
}

template <typename T>
void bTree<T>::rot_less(int index)
{
    int cur = index;
    int l = 2*cur+1, g = 2*cur+2;
    int gl = 2*g+1, gg = 2*g+2;
    int lg = 2*l+2;
    mVector<tNode<T>*> save1, save2, save3;
    //check if greater less heavy
    if(tree[g]->dep_comp > 0)
    {
        tSlot_N(2*gl+2, save1);
        tSlot_N(gl, save2);
        tSlot_N(g, save3);

        move_subTree(g, save2);
        move_subTree(gg, save3);
        move_subTree(2*gg+1, save1);
        //should not need a situation with only less child
        if(!(tree[2*gg+1]) && tree[2*gg+2])
            tree[gg]->dep_comp = -1;
        else
            tree[gg]->dep_comp = 0;
    }
    //greater greater
    tSlot_N(gl, save1);
    tSlot_N(g, save2);
    tSlot_N(cur, save3);

    move_subTree(cur, save2);
    move_subTree(l, save3);
    move_subTree(lg, save1);

    if(tree[2*l+1] && !(tree[2*l+2]))
        tree[l]->dep_comp = 1;
    else
        tree[l]->dep_comp = 0;

    tree[index]->dep_comp = 0;
}

template <typename T>
void bTree<T>::balance_ins(int index) //balances tree from passed in slot for insert
{
    if(!(tree[index]))
        return;

    int bal_point = index; //loop stops at root or when it finds a -2 or 2
    for(; bal_point && (tree[bal_point]->dep_comp -2) && (tree[bal_point]->dep_comp +2);
        bal_point%2 ? bal_point = (bal_point-1)/2 : bal_point = (bal_point-2)/2);

    if(!(tree[bal_point]->dep_comp -2))
        rot_great(bal_point);
    else if(!(tree[bal_point]->dep_comp +2))
        rot_less(bal_point);
}

template <typename T>
void bTree<T>::balance_rem(int index) //balances tree from passed in slot for remove
{
    if(!(tree[index]))
        return;

    int bal_point = index; //loop stops at root or when it finds a -2 or 2
    for(; bal_point && (tree[bal_point]->dep_comp -2) && (tree[bal_point]->dep_comp +2);
        bal_point%2 ? bal_point = (bal_point-1)/2 : bal_point = (bal_point-2)/2);

    //runs til root
    for(; bal_point +1; bal_point%2 ? bal_point = (bal_point-1)/2 : bal_point = (bal_point-2)/2)
    {
        if(!(tree[bal_point]->dep_comp -2)) //check for rotation and update
        {
            rot_great(bal_point);
            rem_update(bal_point);
        }
        else if(!(tree[bal_point]->dep_comp +2))
        {
            rot_less(bal_point);
            rem_update(bal_point);
        }
    }
}

template <typename T>
void bTree<T>::nukem(int index)  //the boom function
{
    if(!num_node)
        return;
    for(int i = 0; i < tree.size(); i++)
    {
        if(tree[i])
            delete tree[i];

        tree[i] = NULL;
    }

    tree.clear();
    num_node = 0;
    tree.resize(3, NULL);
}

template <typename T>
void bTree<T>::copy(const bTree<T> &other)  //the boom function
{
    if(other.empty())
        return;
    num_node = other.num_node;
    trav = other.trav;
    tree.resize(other.tree.size(), NULL);
    for(int i = 0; i < other.tree.size(); i++)
    {
        if(other.tree[i])
            tree[i] = new tNode<T>(other.tree[i]->data, other.tree[i]->count,
                                   other.tree[i]->dep_comp);
    }
}

template <typename T>
int bTree<T>::depth(int r) //returns the depth of the tree from given root
{
    return !(tree[r]) ? 0 : 1 + max(depth(2*r+1), depth(2*r+2));
}

template <typename T>
T* bTree<T>::dAccess(T &data)  //accessor for data contained in nodes
{
    int i = find(0, data);
    if(i >= 0)
        return &tree[i]->data;
    else
        return NULL;
}

template <typename T>
T& bTree<T>::Access(T &data)  //accessor for data contained in nodes
{
    int i = find(0, data);
    if(i >= 0)
        return tree[i]->data;
    else
        throw 404;
}

template <typename T>
int bTree<T>::find(int r, T &d) //returns slot of node or -1 if not found
{
    while(tree[r])
    {
        if(tree[r]->data == d)
            return r;

        (tree[r]->data > d) ? r = 2*r+1 : r = 2*r+2;
    }

    return -1;
}

template <typename T>
int bTree<T>::findParent(int r, T &d)  //returns existing or possible parent of the item
{
    if(!tree[0])
        return -1;

    for(;tree[r] && (tree[r]->data != d);
        tree[r]->data > d ? r = 2*r+1 : r = 2*r+2);

    r%2 ? r = (r-1)/2 : r = (r-2)/2;

    return r;
}

template <typename T>
int bTree<T>::count()  //returns count
{
    return num_node;
}

template <typename T>
bool bTree<T>::empty()
{
    return !num_node;
}

template <typename T>
int bTree<T>::count(int r)  //returns count given a root
{
    preOrder(r);

    return order.size();
}

template <typename T>
void bTree<T>::move_subTree(int to_r, mVector<tNode<T>*> &temp)
{
    if(temp.empty())
        return;
    mVector<int> v_to;
    v_to.push_back(to_r);
    //cant use tSlot() because of diff requirements
    //but creates a mVector of slots in the same format
    for(int j = 0; 2*j+2 < temp.size(); j++)
    {
        v_to.push_back(2*v_to[j]+1);
        v_to.push_back(2*v_to[j]+2);
    }
    if(v_to[v_to.size()-1]+1 > tree.size())
        tree.resize(v_to[v_to.size()-1]+1, NULL);

    bool stop = false;
    double j = 0, row = 0, powTwo, i;
    for(; !stop; row++)
    {
        powTwo = pow(2,row);
        j += powTwo;
        stop = true; //if we finish a row without doing a move, stop
        for(i = j-powTwo; (i < j) && (i < v_to.size()); i++)
        {
            if(temp[i])
            {
                tree[v_to[i]] = temp[i]; //move to corresponding slot
                stop = false;
            }
        }
    }
}

template <typename T>
void bTree<T>::setSize()  //sets the size of the tree to at least 1 tier below possible leafs
{
    int i = 0;
    for(; tree[i]; i = 2*i+2);
    for(int j = 0; j - 3; j++)
        i = 2*i+2;

    tree.resize(i+1, NULL);
}

template <typename T>
void bTree<T>::tSlot(int r, mVector<int>& v)//returns a mVector holding the slots for the subtree
{
    if(!v.empty())
        v.clear();

    v.push_back(r);
    //starting from root of subtree start listing children
    for(int i = 0; 2*v[i]+2 < tree.size(); i++)
    {
        v.push_back(2*v[i]+1);
        v.push_back(2*v[i]+2);
    }
}

//will create a mVector containing nodes taken from tree
//will be stored in same layout as tslot
template <typename T>
void bTree<T>::tSlot_N(int r, mVector<tNode<T>*>& v)
{
    if(!v.empty())
        v.clear();

    if(!tree[r])
        return;

    mVector<int> v_slots;
    tSlot(r, v_slots);

    bool stop = false;
    double j = 0, row = 0, powTwo, i;
    for(; !stop; row++)
    {
        powTwo = pow(2,row);
        j += powTwo;
        stop = true; //if we finish a row without doing a save, stop
        for(i = j-powTwo; (i < j) && (i < v_slots.size()); i++)
        {
            if(tree[v_slots[i]])
                stop = false;
            v.push_back(tree[v_slots[i]]);
            tree[v_slots[i]] = NULL;
        }
    }
}

template <typename T>
void bTree<T>::setOrder(int index)  //sets order to contain current trav slots
{
    if(!tree[index])
        return;

    if(trav == IN)
        inOrder(index);
    else if(trav == PRE)
        preOrder(index);
    else
        postOrder(index);
}

//outputs in current order with direct pointers to data
template <typename T>
void bTree<T>::getSortedData(mVector<T*> &save)
{
    if(!save.empty())
        save.clear();

    if(!(tree[0]))
        return;

    setOrder(0);

    while(!order.empty())
    {
        save.push_back(&tree[order.front()]->data);
        order.pop();
    }
}

//outputs in current order
template <typename T>
void bTree<T>::getSortedData(mVector<T> &save)
{
    if(!save.empty())
        save.clear();

    if(!(tree[0]))
        return;

    setOrder(0);

    while(!order.empty())
    {
        save.push_back(tree[order.front()]->data);
        order.pop();
    }
}

template <typename T>
void bTree<T>::inOrder (int index)  //returns a queue of slot # to items in-order
{
    stack<int> s;
    while(!order.empty())
        order.pop();

    if(!(tree[index]))
        return;

    int i = index;
    bool fin = false;

    while(!fin)
    {
        if(tree[i])
        {
            s.push(i); //push to stack and move less side
            i = 2*i+1;
        }
        else //reached the end of less for now
        {
            if (!s.empty())
            {
                i = s.top(); //move back to less with item
                s.pop();
                if(tree[i])
                    order.push(i); //store to output queue
                i = 2*i+2;  //move to greater
            }
            else            //set true once stack is
                fin = true; //empty and nothing left to push
        }
    }
}

template <typename T>
void bTree<T>::preOrder (int index)  //returns a queue of slot # to items pre-order
{
    stack<int> s;
    while(!order.empty())
        order.pop();

    if(!(tree[index]))
        return;

    int i;

    s.push(index);  //start with root

    //will simply push reverse order onto stack
    //then pop out in correct order to push into queue
    while(!s.empty())
    {
        i = s.top();
        s.pop();

        order.push(i); //store pointer to queue

        if(2*i+2 < tree.size())
        {           //push greater first
            if(tree[2*i+2])
                s.push(2*i+2); //push less second
            if(tree[2*i+1])
                s.push(2*i+1);
        }
    }
}

template <typename T>
void bTree<T>::postOrder (int index)  //returns a queue of slot # to items post-order
{
    stack<int> s1, s2;
    while(!order.empty())
        order.pop();

    if(!(tree[index]))
        return;

    int i;

    s1.push(index); //start with root of tree so will be at bottom of s2

    //loop pushes tree in post-order backwards so top will be first value
    while(!(s1.empty()))
    {
        i = s1.top();
        s2.push(i);
        s1.pop();

        if(2*i+2 < tree.size())
        {
            if(tree[2*i+1]) //if less exists push first
                s1.push(2*i+1);
            if(tree[2*i+2]) //if greater exists push second
                s1.push(2*i+2);
        }
    }

    //for consistancy push into queue
    while(!(s2.empty()))
    {
        order.push(s2.top());
        s2.pop();
    }
}

#endif // BTREE_H
