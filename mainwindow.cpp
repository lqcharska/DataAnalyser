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
#include <QDir>
#include <QList>


ProgramData programData;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDir dir(".");
    programData.settingsFilePath = (dir.absolutePath() + "/recentFiles.ini");
    programData.saveProgramData.Read(programData.settingsFilePath);

    createCSVMenu();

    QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");
    plot->addGraph();
    plot->addGraph();
    plot->addGraph();
    plot->legend->setVisible(true);
    plot->legend->setFont(QFont("Helvetica", 8));

    QCPTextElement *title = new QCPTextElement(plot, "Interaction Example", QFont("sans", 12, QFont::Bold));
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0, title);
    connect(plot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
    connect(plot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
    connect(title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));

}

MainWindow::~MainWindow()
{
    programData.saveProgramData.Save(programData.settingsFilePath);
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


void MainWindow::openCSV(QString fileName)
{
    if(fileName != "")
    {
        LoadData(fileName);
        programData.saveUserData.CSVfilePath = fileName;
        programData.saveProgramData.PushBackCSV(fileName);
        createCSVMenu();
    }
}


void MainWindow::on_actionOpen_CSV_triggered()
{
   QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open CSV"), "", tr("CSV Files (*.csv)"));
   openCSV(fileName);
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

        programData.saveUserData.Save(fileName);
        programData.saveProgramData.PushBackProject(fileName);
        createProjectMenu();
    }
    else
    {
        programData.saveUserData.Save(programData.projectPath);
    }

}


void MainWindow::on_actionSave_project_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr,
                    "Save Projext", "",
                    "Data analysis (*.da);;All Files (*)");

    programData.saveUserData.Save(fileName);
    programData.saveProgramData.PushBackProject(fileName);
    createProjectMenu();
}


void MainWindow::openProject(QString fileName)
{
    if (fileName != "")
    {
        programData.saveUserData.Read(fileName);
        programData.projectPath = fileName;

        //Put the name of the project to recent projects list
        programData.saveProgramData.PushBackProject(fileName);

        //Create/update project menu
        createProjectMenu();

        //
        QTextBrowser *data =  ui->View->findChild<QTextBrowser*>("rawData");

        //Read user's settings
        data->setFont(programData.saveUserData.fontStyle);
        data->setTextColor(programData.saveUserData.fontColour);
        data->setTextBackgroundColor(programData.saveUserData.backgroundFontColour);
        data->setPalette(programData.saveUserData.backgroundColour);

        //Load recent CSV data
        LoadData(programData.saveUserData.CSVfilePath);

        //Resfresh view
        RefreshDataView();
    }
}


void MainWindow::on_actionOpen_project_triggered()
{
    //Open project
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open Project"), "", tr("DA's Files (*.da)"));
    openProject(fileName);
}


void MainWindow::createCSVMenu()
{
    QMenu *recentFilesMenu = ui->menuFile->findChild<QMenu*>("menuOpen_recent_CSV");
    recentFilesMenu->clear();
    for (auto i: programData.saveProgramData.lastCSV)
    {
         recentFilesMenu->addAction(i);
    }

    for(auto *i: recentFilesMenu->actions())
    {
        QObject::connect(i, &QAction::triggered, this, &MainWindow::openRecentCSV);
    }
}


void MainWindow::createProjectMenu()
{
    QMenu *recentFilesMenu = ui->menuFile->findChild<QMenu*>("menuOpen_recent_project");
    recentFilesMenu->clear();
    for (auto i: programData.saveProgramData.lastProject)
    {
        recentFilesMenu->addAction(i);
    }
    for(auto *i: recentFilesMenu->actions())
    {
        QObject::connect(i, &QAction::triggered, this, &MainWindow::openRecentProject);
    }
}


void MainWindow::openRecentCSV()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        openCSV(action->text());
    }
}


void MainWindow::openRecentProject()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        openProject(action->text());
    }
}



