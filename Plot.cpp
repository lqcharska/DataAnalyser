#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ProgramData.h"


extern ProgramData programData;


void MainWindow::XColumnFill (int arg1)
{
    programData.columnX.clear();
    for (auto i : programData.data.rows)
    {
        programData.columnX.push_back(i.GetValueFromRow(arg1));
    }
    refreshGraph(0);
    refreshGraph(1);
    refreshGraph(2);
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
    XColumnFill(programData.xColumn);
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

void MainWindow::refreshGraph (int index)
{
    QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");

    programData.graphs[index].values.clear();
    for (auto i : programData.data.rows)
    {
        programData.graphs[index].values.push_back(i.GetValueFromRow(programData.graphs[index].graphColumnIndex));
    }
    plot->graph(index)->setData(programData.columnX,programData.graphs[index].values);
    plot->graph(index)->setPen(QPen(programData.graphs[index].colour));
    plot->graph(index)->setName(programData.data.listOfTitles[programData.graphs[index].graphColumnIndex]);
    //plot->graph(1)->setScatterStyle(QCPScatterStyle::ssCircle);

    //connect(plot->xAxis, SIGNAL(rangeChanged(QCPRange)), plot->xAxis2, SLOT(setRange(QCPRange)));
    //connect(plot->yAxis, SIGNAL(rangeChanged(QCPRange)), plot->yAxis2, SLOT(setRange(QCPRange)));
    //plot->graph(0)->rescaleAxes();
    //plot->graph(1)->rescaleAxes(true);

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    plot->replot();
}


void setGraphColumnIndex(int columnIndex, int graphIndex)
{
    if(columnIndex < programData.data.rows.at(1).GetLengthOfRow())
    {
        programData.graphs[graphIndex].graphColumnIndex = columnIndex;
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("You do not have enough number of columns");
        msgBox.setInformativeText("Max is: " + QString::number(programData.data.rows.at(1).GetLengthOfRow()));
        msgBox.exec();
        programData.graphs[graphIndex].graphColumnIndex = (programData.data.rows.at(1).GetLengthOfRow() - 1);
    }
}

void MainWindow::on_YColumn1_valueChanged(int arg1)
{
    setGraphColumnIndex(arg1, 0);
    refreshGraph(0);
}

void MainWindow::on_YColumn2_valueChanged(int arg1)
{
    setGraphColumnIndex(arg1, 1);
    refreshGraph(1);
}

void MainWindow::on_YColumn3_valueChanged(int arg1)
{
    setGraphColumnIndex(arg1, 2);
    refreshGraph(2);
}


void MainWindow::setGraphColour (int graphIndex)
{
    QColor colour = QColorDialog::getColor(Qt::white, this, "Choose colour of your graph");
    if(colour.isValid())
    {
         programData.graphs[graphIndex].colour = colour;
    }
}


void MainWindow::on_ColourGraph1_clicked()
{
    setGraphColour(0);
    refreshGraph(0);
}

void MainWindow::on_ColourGraph2_clicked()
{
    setGraphColour(1);
    refreshGraph(1);
}

void MainWindow::on_ColourGraph3_clicked()
{
    setGraphColour(2);
    refreshGraph(2);
}


void MainWindow::setZoom (int graphIndex)
{
    QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");
    double xMin = *std::min_element(programData.columnX.begin(), programData.columnX.end());
    double xMax = *std::max_element(programData.columnX.begin(), programData.columnX.end());
    double yMin = *std::min_element(programData.graphs[graphIndex].values.begin(), programData.graphs[graphIndex].values.end());
    double yMax = *std::max_element(programData.graphs[graphIndex].values.begin(), programData.graphs[graphIndex].values.end());
    plot->xAxis->setRange(xMin, xMax);
    plot->yAxis->setRange(yMin, yMax);
    plot->replot();
}


void MainWindow::on_ZoomGraph1_clicked()
{
    setZoom(0);
}

void MainWindow::on_ZoomGraph2_clicked()
{
    setZoom(1);
}

void MainWindow::on_ZoomGraph3_clicked()
{
    setZoom(2);
}


void MainWindow::titleDoubleClick(QMouseEvent* event)
{
  QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");
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


