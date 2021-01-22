#pragma once
#include <QString>
#include "CSVData.h"
#include "SaveProject.h"

class ProgramData
{
public:
    CSVData data;
    QVector<double> columnX;
    QVector<double> columnY;
    int xColumn = 0;
    int yColumn = 0;
    SaveManager saveManager;
    QString projectPath;
};
