#ifndef GAUSSMODEL_H
#define GAUSSMODEL_H

#include <QList>

class GaussModel
{
    QList<int> _entries;
    int _currentWinner;
public:
    GaussModel(int size);
    void reset();
    void add(int value);
    int winner();
};

#endif // GAUSSMODEL_H
