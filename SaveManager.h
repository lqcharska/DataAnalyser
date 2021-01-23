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
protected:

    virtual void Save (QString) = 0;
    virtual void Read(QString) = 0;


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


};
