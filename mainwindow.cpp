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


           // to witek dorzuca
           programData.data.render();

           programData.data.close();
       }

   }

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

void MainWindow::on_actionDay_mode_triggered()
{
    QPalette Palette;
    qApp->setPalette(Palette);
    RefreshDataView();
}
