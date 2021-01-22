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
#include <algorithm>


ProgramData programData;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow:: RefreshDataView()
{
    QTextBrowser *rawData =  ui->View->findChild<QTextBrowser*>("rawData");
    QString temporaryData = rawData->toPlainText();
    rawData->clear();
    rawData->append(temporaryData);
}

void MainWindow:: LoadData (QString fileName)
{
    if  (!fileName.isEmpty())
    {
        programData.data.clear();
        programData.data.setFileName(fileName);
        if(!programData.data.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox msgBox;
            msgBox.setText("Upsi... Cannot open the file!");
            msgBox.exec();
        }
        else
        {
            QTextBrowser *rawData =  ui->View->findChild<QTextBrowser*>("rawData");
            rawData->clear();
            rawData->append(programData.data.readAll());
            programData.data.render();
            programData.data.close();
        }

    }
}

void MainWindow::on_actionOpen_CSV_triggered()
{
   QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open CSV"), "", tr("CSV Files (*.csv)"));
   LoadData(fileName);
   programData.saveManager.CSVfilePath = fileName;

    /*QMessageBox msgBox;
    msgBox.setText(fileName);
    msgBox.exec();*/

}


//dark mode from https://www.medo64.com/2020/08/dark-mode-for-qt-application/
void MainWindow::on_actionNight_mode_triggered()
{
    qApp->setStyle(QStyleFactory::create("Fusion"));

    QPalette newPalette;
    newPalette.setColor(QPalette::Window,          QColor( 37,  37,  37));
    newPalette.setColor(QPalette::WindowText,      QColor(212, 212, 212));
    newPalette.setColor(QPalette::Base,            QColor( 60,  60,  60));
    newPalette.setColor(QPalette::AlternateBase,   QColor( 45,  45,  45));
    newPalette.setColor(QPalette::PlaceholderText, QColor(127, 127, 127));
    newPalette.setColor(QPalette::Text,            QColor(212, 212, 212));
    newPalette.setColor(QPalette::Button,          QColor( 45,  45,  45));
    newPalette.setColor(QPalette::ButtonText,      QColor(212, 212, 212));
    newPalette.setColor(QPalette::BrightText,      QColor(240, 240, 240));
    newPalette.setColor(QPalette::Highlight,       QColor( 38,  79, 120));
    newPalette.setColor(QPalette::HighlightedText, QColor(240, 240, 240));

    newPalette.setColor(QPalette::Light,           QColor( 60,  60,  60));
    newPalette.setColor(QPalette::Midlight,        QColor( 52,  52,  52));
    newPalette.setColor(QPalette::Dark,            QColor( 30,  30,  30) );
    newPalette.setColor(QPalette::Mid,             QColor( 37,  37,  37));
    newPalette.setColor(QPalette::Shadow,          QColor( 0,    0,   0));

    newPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));

    qApp->setPalette(newPalette);
    RefreshDataView();
}


//day mode
void MainWindow::on_actionDay_mode_triggered()
{
    QPalette Palette;
    qApp->setPalette(Palette);
    RefreshDataView();
}




void MainWindow::on_actionSave_project_triggered()
{
    if (programData.projectPath == "")
    {
        QString fileName = QFileDialog::getSaveFileName(nullptr,
                        "Save Projext", "",
                        "Data analysis (*.da);;All Files (*)");

        programData.saveManager.Save(fileName);
    }
    else
    {
        programData.saveManager.Save(programData.projectPath);
    }

}


void MainWindow::on_actionSave_project_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr,
                    "Save Projext", "",
                    "Data analysis (*.da);;All Files (*)");

    programData.saveManager.Save(fileName);
}



void XColumnFill (int arg1)
{
    programData.columnX.clear();
    for (auto i : programData.data.rows)
    {
        programData.columnX.push_back(i.GetValueFromRow(arg1));
    }
}

void YColumnFill (int arg1)
{
    programData.columnY.clear();
    for (auto i : programData.data.rows)
    {
        programData.columnY.push_back(i.GetValueFromRow(arg1));
    }
}

void MainWindow::on_actionPlot_data_triggered()
{
    QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");
    XColumnFill(programData.xColumn);
    YColumnFill(programData.yColumn);
    plot->addGraph();
    plot->graph(0)->setData(programData.columnX,programData.columnY);
    // give the axes some labels:
    plot->xAxis->setLabel("x");
    plot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    plot->xAxis->setRange(*std::min_element(programData.columnX.begin(), programData.columnX.end()), *std::max_element(programData.columnX.begin(), programData.columnX.end()));
    plot->yAxis->setRange(*std::min_element(programData.columnY.begin(), programData.columnY.end()), *std::max_element(programData.columnY.begin(), programData.columnY.end()));
    plot->replot();

}



void MainWindow::on_ChooseXColumn_valueChanged(int arg1)
{
    if(arg1 < programData.data.rows.at(1).GetLengthOfRow())
    {
        programData.xColumn = arg1;

    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("You do not have enough number of columns");
        msgBox.setInformativeText("Max is: " + QString::number(programData.data.rows.at(1).GetLengthOfRow()));
        msgBox.exec();
        programData.xColumn = (programData.data.rows.at(1).GetLengthOfRow() - 1);
    }

}

void MainWindow::on_ChooseYColumn_valueChanged(int arg1)
{
    if(arg1 < programData.data.rows.at(1).GetLengthOfRow())
    {
        programData.yColumn = arg1;
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("You do not have enough number of columns");
        msgBox.setInformativeText("Max is: " + QString::number(programData.data.rows.at(1).GetLengthOfRow()));
        msgBox.exec();
        programData.yColumn = (programData.data.rows.at(1).GetLengthOfRow() - 1);
    }

}

void MainWindow::on_actionOpen_project_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open Project"), "", tr("DA's Files (*.da)"));
    programData.saveManager.Read(fileName);
    programData.projectPath = fileName;
    QTextBrowser *data =  ui->View->findChild<QTextBrowser*>("rawData");
    data->setFont(programData.saveManager.fontStyle);
    data->setTextColor(programData.saveManager.fontColour);
    data->setTextBackgroundColor(programData.saveManager.backgroundFontColour);
    data->setPalette(programData.saveManager.backgroundColour);
    LoadData(programData.saveManager.CSVfilePath);
    RefreshDataView();


}
