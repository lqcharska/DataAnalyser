#pragma once
#include <QVector>
#include <QColor>

class Graph
{
public:
    bool visible;
    int graphColumnIndex;
    QVector<double> values;
    QColor colour;
    // QString label;

    void clear()
    {
       values.clear();
    }
};
