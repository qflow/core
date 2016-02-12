#include "gaussmodel.h"

GaussModel::GaussModel(int size)
{
    _entries.reserve(size);
    for(int i=0; i<size; i++)
    {
        _entries.append(0);
    }
}
void GaussModel::reset()
{
    for(int i=0; i<_entries.length(); i++)
    {
        _entries[i] = 0;
    }
}
void GaussModel::add(int value)
{
    _entries[value]++;
    if(_entries[value] > _entries[_currentWinner]) _currentWinner = value;

}
int GaussModel::winner()
{
    return _currentWinner;
}
