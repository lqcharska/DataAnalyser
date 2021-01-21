#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_CSV_triggered();

    void on_actionFont_s_style_triggered();

    void on_actionBackground_s_colour_triggered();

    void on_actionFont_s_colour_triggered();

    void on_actionBackground_font_s_colour_triggered();

    void on_actionNight_mode_triggered();

    void on_actionDay_mode_triggered();

    void RefreshDataView();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
