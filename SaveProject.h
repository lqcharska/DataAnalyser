#pragma once
#include <vector>
#include <QString>
#include  <string>
#include <QFile>
#include <QFont>
#include <QColor>
#include <QVariant>
#include <QFileDialog>
#include <QTextStream>


class SaveManager
{
    QString SaveBool(QString name, bool b)
    {
        return "#" + name + " " + QVariant(b).toString() + "\n";
    }

    QString SaveNumber(QString name, int i)
    {
        return "#" + name + " " + QString::number(i) + "\n";
    }

    QString SaveColour(QString name, QColor colour)
    {
        return "#" + name + " " + QString::number(colour.red()) +" " + QString::number(colour.green()) + " " + QString::number(colour.blue()) + "\n";
    }

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

     QString readStringFromLine (QString name, QByteArray line)
     {
         QString lineString = QString(line);
         if ( lineString.indexOf("#" + name) == 0)
         {
             int length = 1 + name.length() + 1;
             return lineString.mid(length).chopped(1);
         }
         return "";

     }

     int readIntFromLine (QString name, QByteArray line)
     {
         QString input = readStringFromLine(name, line);
         if (input != "")
         {
             return input.toInt();
         }
         return 0;
     }

     int readBoolFromLine (QString name, QByteArray line)
     {
         QString input = readStringFromLine(name, line);
         if (input != "")
         {
             return QVariant(input).toBool();
         }
         return 0;
     }

     QColor readColourFromLine (QString name, QByteArray line)
     {
         QString input = readStringFromLine(name, line);
         if (input != "")
         {
             QStringList colours = input.split(' ');
             int r = colours[0].toInt();
             int g = colours[1].toInt();
             int b = colours[2].toInt();
             return QColor(r,g,b);
         }
         return QColor(0,0,0);
     }


     void Read(QString projectPath)
     {
         QFile file(projectPath);
         if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
               return;
         //while(!file.atEnd())
         //{
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
         //}








            file.close();
     }






};
