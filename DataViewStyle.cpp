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


extern ProgramData programData;

//Changing style of data (font, colour of background) panel through menu

void MainWindow::on_actionFont_s_style_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
    {
        ui->View->findChild<QTextBrowser*>("rawData")->setFont(font);
        programData.saveManager.fontStyle = font;
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
         programData.saveManager.backgroundColour = colour;
    }
    RefreshDataView();
}

void MainWindow::on_actionFont_s_colour_triggered()
{
    QColor colour = QColorDialog::getColor(Qt::white, this, "Choose colour of your background");
    if(colour.isValid())
    {
         ui->View->findChild<QTextBrowser*>("rawData")->setTextColor(colour);
         programData.saveManager.fontColour = colour;
    }
    RefreshDataView();
}

void MainWindow::on_actionBackground_font_s_colour_triggered()
{
    QColor colour = QColorDialog::getColor(Qt::white, this, "Choose colour of your background");
    if(colour.isValid())
    {
         ui->View->findChild<QTextBrowser*>("rawData")->setTextBackgroundColor(colour);
         programData.saveManager.backgroundFontColour = colour;
    }
    RefreshDataView();
}
