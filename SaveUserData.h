#pragma once
#include "SaveManager.h"

class SaveUserData : public SaveManager
{
public:
    QString CSVfilePath;

    QFont fontStyle;
    QColor fontColour;
    QColor backgroundFontColour;

    QColor backgroundColour;

    void Save(QString fileName)
    {
        QString saveData = "";
        saveData = saveData + "#filepath " + CSVfilePath + "\n";
        saveData += "\n";
        saveData += "#fontstyle " + fontStyle.family() + "\n";
        saveData += SaveNumber("fontsize", fontStyle.pointSize());
        saveData += SaveBool("fontunderline", fontStyle.underline());
        saveData += SaveBool("fontweight", fontStyle.bold());
        saveData += SaveColour("fontcolour", fontColour);
        saveData += SaveColour("fontbackgroundcolour", backgroundFontColour);
        saveData += "\n";
        saveData += SaveColour("backgroundcolour", backgroundColour);

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
        CSVfilePath = readStringFromLine("filepath", file.readLine());
        file.readLine();
        fontStyle.setFamily(readStringFromLine("fonstyle", file.readLine()));
        fontStyle.setPointSize(readIntFromLine("fontsize", file.readLine()));
        fontStyle.setUnderline(readBoolFromLine("fontunderline", file.readLine()));
        fontStyle.setBold(readBoolFromLine("fontweight", file.readLine()));
        fontColour = readColourFromLine ("fontcolour", file.readLine());
        backgroundFontColour = readColourFromLine ("fontbackgroundcolour", file.readLine());
        file.readLine();
        backgroundColour = readColourFromLine("backgroundcolour", file.readLine());
        file.close();
    }
};
