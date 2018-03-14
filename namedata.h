#ifndef NAMEDATA_H
#define NAMEDATA_H

#include <QString>

struct nameData
{
    QString pName;
    int slot;

    friend
    bool operator<(nameData &a, nameData &b)
    {
        return a.pName < b.pName;
    }
    friend
    bool operator>(nameData &a, nameData &b)
    {
        return a.pName > b.pName;
    }
    friend
    bool operator<=(nameData &a, nameData &b)
    {
        return a.pName <= b.pName;
    }
    friend
    bool operator>=(nameData &a, nameData &b)
    {
        return a.pName >= b.pName;
    }
    friend
    bool operator==(nameData &a, nameData &b)
    {
        return a.pName == b.pName;
    }
    friend
    bool operator!=(nameData &a, nameData &b)
    {
        return a.pName != b.pName;
    }
};

#endif // NAMEDATA_H
