#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include <QFileDialog>
#include"ProgramData.h"
#include"QTextBrowser"

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


void MainWindow::on_actionOpen_CSV_triggered()
{
   QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open CSV"), "", tr("CSV Files (*.csv)"));
   if  (!fileName.isEmpty())
   {
       programData.CSVFilePath = fileName;
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
           rawData->append(programData.data.readAll());
       }

   }

    /*QMessageBox msgBox;
    msgBox.setText(fileName);
    msgBox.exec();*/

}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
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
    QString temporaryData = rawData->toPlainText();
    rawData->clear();
    rawData->append(temporaryData);
}
