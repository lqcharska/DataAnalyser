#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include <QFileDialog>
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

    plot->graph(index)->setVisible(true);
    plot->legend->setVisible(true);

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
    QString newTitle = QInputDialog::getText(this, "Change title", "New plot title:", QLineEdit::Normal, title->text(), &ok);
    if (ok)
    {
      title->setText(newTitle);
      plot->replot();
    }
  }
}

void MainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "Change axis label", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      plot->replot();
    }
  }
}

void MainWindow::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
  QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");
  // Rename a graph by double clicking on its legend item
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
  {
    QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    bool ok;
    QString newName = QInputDialog::getText(this, "Change name of graph", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
    if (ok)
    {
      plItem->plottable()->setName(newName);
      plot->replot();
    }
  }
}

void MainWindow::selectionChanged()
{
  QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");

  if (plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || plot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      plot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || plot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    plot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    plot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || plot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      plot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || plot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    plot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    plot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<plot->graphCount(); ++i)
  {
    QCPGraph *graph = plot->graph(i);
    QCPPlottableLegendItem *item = plot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
    }
  }
}

void MainWindow::mousePress()
{
  QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged

  if (plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    plot->axisRect()->setRangeDrag(plot->xAxis->orientation());
  else if (plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    plot->axisRect()->setRangeDrag(plot->yAxis->orientation());
  else
    plot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::mouseWheel()
{
  QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed

  if (plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    plot->axisRect()->setRangeZoom(plot->xAxis->orientation());
  else if (plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    plot->axisRect()->setRangeZoom(plot->yAxis->orientation());
  else
    plot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::removeSelectedGraph()
{
  QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");
  if (plot->selectedGraphs().size() > 0)
  {
      plot->selectedGraphs().first()->setVisible(false);
      plot->replot();
  }
}

void MainWindow::removeAllGraphs()
{
  QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");
  plot->graph(0)->setVisible(false);
  plot->graph(1)->setVisible(false);
  plot->graph(2)->setVisible(false);
  plot->replot();
//  programData.graphs[0].clear();
//  programData.graphs[1].clear();
//  programData.graphs[2].clear();
//  programData.columnX.clear();
//  programData.columnY.clear();
//  programData.xColumn = 0;
//  programData.yColumn = 0;
//  refreshGraph(0);
//  refreshGraph(1);
//  refreshGraph(2);
}

void MainWindow::moveLegend()
{
  QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");
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

void MainWindow::contextMenuRequest(QPoint pos)
{
  QCustomPlot *plot =  ui->View->findChild<QCustomPlot*>("Plot");
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);

  if (plot->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  }
  else
  {
    if (plot->selectedGraphs().size() > 0)
    {
        menu->addAction("Hide selected graph", this, SLOT(removeSelectedGraph()));
    }
    if (plot->graphCount() > 0)
    {
        menu->addAction("Hide all graphs", this, SLOT(removeAllGraphs()));
    }
  }
  menu->popup(plot->mapToGlobal(pos));
}

void MainWindow::graphClicked(QCPAbstractPlottable *plottable, int dataIndex)
{
  // since we know we only have QCPGraphs in the plot, we can immediately access interface1D()
  // usually it's better to first check whether interface1D() returns non-zero, and only then use it.
  double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
  QString message = QString("Clicked on graph '%1' at data point #%2 with value %3.").arg(plottable->name()).arg(dataIndex).arg(dataValue);
  ui->statusbar->showMessage(message, 2500);
}
