#include "resultstruct.h"

ResultStruct::ResultStruct()
{
    res = 0;
    pIndex = 0;
}

ResultStruct::ResultStruct(double newRes, int newPIndex)
{
    res = newRes;
    pIndex = newPIndex;
}

ResultStruct::ResultStruct(const ResultStruct &other)
{
    res = other.res;
    pIndex = other.pIndex;
}




