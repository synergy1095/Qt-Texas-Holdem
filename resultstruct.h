#ifndef RESULTSTRUCT_H
#define RESULTSTRUCT_H

class ResultStruct
{
public:
    ResultStruct();
    ResultStruct(double newRes, int newPIndex);
    ResultStruct(const ResultStruct &other);


    friend
    bool operator<(ResultStruct &a, ResultStruct &b)
    {
        return a.res < b.res;
    }
    friend
    bool operator>(ResultStruct &a, ResultStruct &b)
    {
        return a.res > b.res;
    }
    friend
    bool operator<=(ResultStruct &a, ResultStruct &b)
    {
        return a.res <= b.res;
    }
    friend
    bool operator>=(ResultStruct &a, ResultStruct &b)
    {
        return a.res >= b.res;
    }
    friend
    bool operator==(ResultStruct &a, ResultStruct &b)
    {
        return a.res == b.res;
    }
    friend
    bool operator!=(ResultStruct &a, ResultStruct &b)
    {
        return a.res != b.res;
    }


    double res; //Hand result
    int pIndex; //Player index
};

#endif // RESULTSTRUCT_H
