#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include <QFileDialog>
#include "ProgramData.h"
#include "QTextBrowser"
#include <QColorDialog>
#include <QColor>
#include <QFontDialog>
#include <QFont>
#include <QStyleFactory>


//Changing style of data (font, colour of background) through user's panel

void MainWindow::on_SetItalic_stateChanged(int arg1)
{
    QTextBrowser *rawData =  ui->View->findChild<QTextBrowser*>("rawData");
    if ( arg1 == 2)
    {
        rawData->setFontItalic(QFont::StyleItalic);

    }
    else if (arg1 == 0)
    {
        rawData->setFontItalic(QFont::StyleNormal);
    }
    RefreshDataView();
}


void MainWindow::on_SetBold_stateChanged(int arg1)
{
    QTextBrowser *rawData =  ui->View->findChild<QTextBrowser*>("rawData");
    if ( arg1 == 2)
    {
        rawData->setFontWeight(QFont::Bold);
    }
    else if (arg1 == 0)
    {
        rawData->setFontWeight(QFont::Normal);
    }
    RefreshDataView();
}

void MainWindow::on_SetFont_currentIndexChanged(int index)
{
    int cambria = 0;
    int timesNewRoman = 1;
    int arial = 2;
    QTextBrowser *rawData =  ui->View->findChild<QTextBrowser*>("rawData");
    if(index == cambria)
    {
        rawData->setFontFamily("Cambria");
    }
    else if (index == timesNewRoman)
    {
        rawData->setFontFamily("Times New Roman");
    }
    else if (index == arial)
    {
        rawData->setFontFamily("Arial");
    }
    RefreshDataView();
}

void MainWindow::on_SetFont_activated(int index)
{
    int cambria = 0;
    int timesNewRoman = 1;
    int arial = 2;
    QTextBrowser *rawData =  ui->View->findChild<QTextBrowser*>("rawData");
    if(index == cambria)
    {
        rawData->setFontFamily("Cambria");
    }
    else if (index == timesNewRoman)
    {
        rawData->setFontFamily("Times New Roman");
    }
    else if (index == arial)
    {
        rawData->setFontFamily("Arial");
    }
    RefreshDataView();
}

void MainWindow::on_SetFondSize_valueChanged(int arg1)
{
    QTextBrowser *rawData =  ui->View->findChild<QTextBrowser*>("rawData");
    rawData->setFontPointSize(arg1);
    RefreshDataView();
}

void MainWindow::on_SetBackgroundColour_clicked()
{
    QColor colour = QColorDialog::getColor(Qt::white, this, "Choose colour of your background");
    if(colour.isValid())
    {
        QTextBrowser *rawData =  ui->View->findChild<QTextBrowser*>("rawData");
        rawData->setPalette(colour);
        RefreshDataView();
    }
}

void MainWindow::on_SetFontColour_clicked()
{
    QColor colour = QColorDialog::getColor(Qt::white, this, "Choose colour of your font");
    if(colour.isValid())
    {
        QTextBrowser *rawData =  ui->View->findChild<QTextBrowser*>("rawData");
        rawData->setTextColor(colour);
        RefreshDataView();
    }
}




//Changing style of data (font, colour of background) panel through menu

void MainWindow::on_actionFont_s_style_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
    {
        ui->View->findChild<QTextBrowser*>("rawData")->setFont(font);
    }
    else return;
    RefreshDataView();
}

void MainWindow::on_actionBackground_s_colour_triggered()
{
    QColor colour = QColorDialog::getColor(Qt::white, this, "Choose colour of your background");
    if(colour.isValid())
    {
         ui->View->findChild<QTextBrowser*>("rawData")->setPalette(colour);
    }
    RefreshDataView();
}

void MainWindow::on_actionFont_s_colour_triggered()
{
    QColor colour = QColorDialog::getColor(Qt::white, this, "Choose colour of your background");
    if(colour.isValid())
    {
         ui->View->findChild<QTextBrowser*>("rawData")->setTextColor(colour);
    }
    RefreshDataView();
}

void MainWindow::on_actionBackground_font_s_colour_triggered()
{
    QColor colour = QColorDialog::getColor(Qt::white, this, "Choose colour of your background");
    if(colour.isValid())
    {
         ui->View->findChild<QTextBrowser*>("rawData")->setTextBackgroundColor(colour);
    }
    RefreshDataView();
}
