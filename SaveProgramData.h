#pragma once
#include "SaveManager.h"
#include <QStringList>
#include <QMessageBox>

class SaveProgramData : public SaveManager
{
public:
    QStringList lastCSV;
    QStringList lastProject;

    void PushBackCSV(QString fileName)
    {
        for (int i = 0; i < lastCSV.size(); i++)
        {
            if(lastCSV[i] == fileName)
            {
                lastCSV.removeAt(i);
                break;
            }
        }
        lastCSV.push_back(fileName);
        if (lastCSV.size() > 5)
        {
            lastCSV.pop_front();
        }
    }

    void PushBackProject(QString fileName)
    {
        for (int i = 0; i < lastProject.size(); i++)
        {
            if(lastProject[i] == fileName)
            {
                lastProject.removeAt(i);
                break;
            }
        }
        lastProject.push_back(fileName);
        if (lastProject.size() > 5)
        {
            lastProject.pop_front();
        }
    }

    void Save(QString fileName)
    {
        QString saveData = "";
        for (auto i : lastCSV)
        {
            saveData += "#" + i + "\n";
        }
        saveData += "\n";
        for (auto i : lastProject)
        {
            saveData += "$" + i + "\n";
        }
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
              return;
        QTextStream out(&file);
        out<< saveData;
        file.close();
    }
    void Read(QString fileName)
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return;
        }
        while(!file.atEnd())
        {
            QString lineString = QString(file.readLine());
            if (lineString.indexOf("#") == 0)
            {
                int length = 1;
                lastCSV.push_back(lineString.mid(length).chopped(1));
            }
            if (lineString.indexOf("$") == 0)
            {
                int length = 1;
                lastProject.push_back(lineString.mid(length).chopped(1));
            }
        }
        file.close();
    }

};
