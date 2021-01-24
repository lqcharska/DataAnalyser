#pragma once
#include <QString>
#include "CSVData.h"
#include "SaveUserData.h"
#include "SaveProgramData.h"
#include "Graph.h"

class ProgramData
{
public:
    CSVData data;
    QVector<double> columnX; //data from x column
    int xColumn = 0; //X axis column selected by user
    SaveUserData saveUserData; //users settings
    QString projectPath; //path to project
    QString settingsFilePath; //path settings
    SaveProgramData saveProgramData; //recent users projects

    std::array<Graph, 3> graphs;
};
