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
    if(programData.saveUserData.CSVfilePath != "")
    {
        QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");
        XColumnFill(programData.xColumn);
        YColumnFill(programData.yColumn);
        plot->addGraph();
        plot->graph(0)->setPen(QPen(Qt::blue));
        plot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
        plot->graph(0)->setData(programData.columnX,programData.columnY);
        // give the axes some labels:
        programData.data.listOfTitles[programData.xColumn];
        plot->xAxis->setLabel(programData.data.listOfTitles[programData.xColumn]);
        plot->yAxis->setLabel(programData.data.listOfTitles[programData.yColumn]);
        // set axes ranges, so we see all data:
        plot->xAxis->setRange(*std::min_element(programData.columnX.begin(), programData.columnX.end()), *std::max_element(programData.columnX.begin(), programData.columnX.end()));
        plot->yAxis->setRange(*std::min_element(programData.columnY.begin(), programData.columnY.end()), *std::max_element(programData.columnY.begin(), programData.columnY.end()));
        QFont legendFont = font();

        plot->plotLayout()->insertRow(0);
        QCPTextElement *title = new QCPTextElement(plot, "Your data", QFont("sans", 17, QFont::Bold));
        plot->plotLayout()->addElement(0, 0, title);

        legendFont.setPointSize(10);
        plot->legend->setVisible(true);
        plot->legend->setFont(legendFont);
        plot->legend->setSelectedFont(legendFont);
        plot->legend->setSelectableParts(QCPLegend::spItems);
        plot->replot();
    }


}



void MainWindow::on_actionAdd_plot_triggered()
{
    QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");
    XColumnFill(programData.xColumn);
    YColumnFill(programData.yColumn);
    plot->addGraph();
    plot->graph(1)->setData(programData.columnX,programData.columnY);
    plot->graph(1)->setPen(QPen(Qt::red));
    plot->graph(1)->setScatterStyle(QCPScatterStyle::ssCircle);
    plot->xAxis->setLabel("x");
    plot->yAxis->setLabel("y");
    plot->xAxis2->setVisible(true);
    plot->xAxis2->setTickLabels(false);
    plot->yAxis2->setVisible(true);
    plot->yAxis2->setTickLabels(false);
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(plot->xAxis, SIGNAL(rangeChanged(QCPRange)), plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(plot->yAxis, SIGNAL(rangeChanged(QCPRange)), plot->yAxis2, SLOT(setRange(QCPRange)));
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    plot->graph(0)->rescaleAxes();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    plot->graph(1)->rescaleAxes(true);
    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    plot->replot();
}

void MainWindow::titleDoubleClick(QMouseEvent* event)
{
  QCustomPlot *plot =ui->View->findChild<QCustomPlot*>("Plot");
  Q_UNUSED(event)
  if (QCPTextElement *title = qobject_cast<QCPTextElement*>(sender()))
  {
    // Set the plot title by double clicking on it
    bool ok;
    QString newTitle = QInputDialog::getText(this, "QCustomPlot example", "New plot title:", QLineEdit::Normal, title->text(), &ok);
    if (ok)
    {
      title->setText(newTitle);
      plot->replot();
    }
  }
}

void MainWindow::removeSelectedGraph()
{
  QCustomPlot *plot =ui->View->findChild<QCustomPlot*>("Plot");
  if (plot->selectedGraphs().size() > 0)
  {
    plot->removeGraph(plot->selectedGraphs().first());
    plot->replot();
  }
}

void MainWindow::moveLegend()
{
  QCustomPlot *plot =ui->View->findChild<QCustomPlot*>("Plot");
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
     plot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
     plot->replot();
    }
  }
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





