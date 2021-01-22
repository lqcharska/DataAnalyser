#pragma once
#include "QString"
#include <QFile>
#include <vector>
#include <QMessageBox>


class Row
{
private:
    bool valid = true;
public:
    std::vector<float> content;
    Row  (QByteArray line)
    {
        QString LineAsString = QString(line);
        QStringList listOfLine = LineAsString.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        for (auto i:listOfLine)
        {
            bool ok;
            float item = i.toFloat(&ok);
            if ( ok == false)
            {
                valid = false;
            }
            else
            {
                content.push_back(item);
            }
        }

    }

    float GetValueFromRow(int i)
    {
        return content.at(i);
    }

    int GetLengthOfRow()
    {
        return content.size();
    }

    bool IsValid()
    {
       return valid;
    }
    int ColumnsCount()
    {
       return content.size();
    }
};

/*QMessageBox msgBox;
msgBox.setText(fileName);
msgBox.exec();*/


class CSVData: public QFile
{
public:
    std::vector<Row> rows;
    int damagedRows = 0;
    QStringList listOfTitles;

    Row getRows(int i)
    {
        return rows.at(i);
    }

    bool render()
    {
        seek(0);
        int i = 0;
        while(!atEnd())
        {
            QByteArray line = readLine();
            if( line[0] == '#')
            {
                i++;
            }
            else
            {
                Row row(line);
                if (row.IsValid())
                {
                    rows.push_back(row);
                }
                else
                {
                    damagedRows ++;
                }
            }
        }
        seek(0);
        for (int x = 0; x < (i-2); x++ )
        {
            readLine();
        }
        QString title = QString(readLine());
        listOfTitles = title.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        listOfTitles.removeAt(0);
        bool valid = Check();

        if (damagedRows != 0 || !valid)
        {
            QMessageBox msgBox;
            if(!valid && damagedRows == 0)
            {
                msgBox.setText("It is not CSV file :/ ");
            }
            if(damagedRows != 0 && valid)
            {
                msgBox.setText("Number of damaged rows: " + QString::number(damagedRows));
            }
            else if (!valid && damagedRows != 0)
            {
                msgBox.setText("Blimey! It is not CSV file and number of damaged rows is: " + QString::number(damagedRows));
            }
            msgBox.exec();
            rows.clear();
            return false;
        }
        return true;
    }

    bool Check()
    {
        for (auto i:rows)
        {
            if(listOfTitles.length() != i.ColumnsCount())
            {
                return false;
            }
        }
        return true;
    }

    void clear()
    {
       flush();
       rows.clear();
       damagedRows = 0;
       listOfTitles.clear();
    }
};
