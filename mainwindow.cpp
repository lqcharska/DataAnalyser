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

    setWindowTitle("No name for now");

    QDir dir(".");
    programData.settingsFilePath = (dir.absolutePath() + "/recentFiles.ini");
    programData.saveProgramData.Read(programData.settingsFilePath);

    createCSVMenu();

    QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);
    plot->addGraph();
    plot->addGraph();
    plot->addGraph();

    plot->xAxis->setRange(-8, 8);
    plot->yAxis->setRange(-5, 5);
    plot->axisRect()->setupFullAxesBox();

    plot->legend->setVisible(true);
    plot->legend->setFont(QFont("Helvetica", 8));
    plot->legend->setSelectableParts(QCPLegend::spItems);

    plot->xAxis->setLabel("x Axis");
    plot->yAxis->setLabel("y Axis");

    QCPTextElement *title = new QCPTextElement(plot, "Interaction Example", QFont("sans", 12, QFont::Bold));
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0, title);


    // connect slot that ties some axis selections together (especially opposite axes):
      connect(plot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
      // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
      connect(plot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
      connect(plot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

      // make bottom and left axes transfer their ranges to top and right axes:
      connect(plot->xAxis, SIGNAL(rangeChanged(QCPRange)), plot->xAxis2, SLOT(setRange(QCPRange)));
      connect(plot->yAxis, SIGNAL(rangeChanged(QCPRange)), plot->yAxis2, SLOT(setRange(QCPRange)));

      // connect some interaction slots:
      connect(plot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
      connect(plot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
      connect(title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));

      // setup policy and connect slot for context menu popup:
      plot->setContextMenuPolicy(Qt::CustomContextMenu);
      connect(plot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

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
        activateIcons();
    }
}


void MainWindow::activateIcons()
{
    QSpinBox *ChooseXColumn =  ui->Properties->findChild<QSpinBox*>("ChooseXColumn");
    ChooseXColumn->setEnabled(true);

    QSpinBox *YColumn1 =  ui->Properties->findChild<QSpinBox*>("YColumn1");
    YColumn1->setEnabled(true);
    QPushButton *ColourGraph1 =  ui->Properties->findChild<QPushButton*>("ColourGraph1");
    ColourGraph1->setEnabled(true);
    QPushButton *ZoomGraph1 =  ui->Properties->findChild<QPushButton*>("ZoomGraph1");
    ZoomGraph1->setEnabled(true);

    QSpinBox *YColumn2 =  ui->Properties->findChild<QSpinBox*>("YColumn2");
    YColumn2->setEnabled(true);
    QPushButton *ColourGraph2 =  ui->Properties->findChild<QPushButton*>("ColourGraph2");
    ColourGraph2->setEnabled(true);
    QPushButton *ZoomGraph2 =  ui->Properties->findChild<QPushButton*>("ZoomGraph2");
    ZoomGraph2->setEnabled(true);

    QSpinBox *YColumn3 =  ui->Properties->findChild<QSpinBox*>("YColumn3");
    YColumn3->setEnabled(true);
    QPushButton *ColourGraph3 =  ui->Properties->findChild<QPushButton*>("ColourGraph3");
    ColourGraph3->setEnabled(true);
    QPushButton *ZoomGraph3 =  ui->Properties->findChild<QPushButton*>("ZoomGraph3");
    ZoomGraph3->setEnabled(true);
}

void MainWindow::on_actionOpen_CSV_triggered()
{
   QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open CSV"), "", tr("CSV Files (*.csv)"));
   openCSV(fileName);
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
        setWindowTitle(fileName);
    }
}


void MainWindow::on_actionOpen_project_triggered()
{
    //Open project
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open Project"), "", tr("DA's Files (*.da)"));
    openProject(fileName);

    activateIcons();
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
        activateIcons();
    }
}


void MainWindow::openRecentProject()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        openProject(action->text());
        activateIcons();
    }
}




void MainWindow::on_actionClean_triggered()
{
    QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");
    programData.data.rows.clear();
    programData.graphs[0].clear();
    programData.graphs[1].clear();
    programData.graphs[2].clear();
    programData.columnX.clear();
    programData.xColumn = 0;
    refreshGraph(0);
    refreshGraph(1);
    refreshGraph(2);
    plot->legend->setVisible(false);
    plot->replot();

    ui->View->findChild<QTextBrowser*>("rawData")->clear();
}
