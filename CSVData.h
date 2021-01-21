#pragma once
#include "QString"
#include <QFile>
#include <vector>


class Row
{
public:
    int index;
    std::vector<float> content;
};


class CSVData: public QFile
{
public:
    std::vector<Row> rows;

    bool render() {
        // TODO
        return true;
    }

    void clear() {
        // TODO
    }
};
