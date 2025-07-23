#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chartWidget.h"
#include <QMainWindow>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QProcess* hardwareReaderProcess = nullptr;
    chartWidget *cpuUsageChartWidget;
    chartWidget *cpuTempChartWidget;
    chartWidget *gpuUsageChartWidget;
    chartWidget *gpuTempChartWidget;
};
#endif // MAINWINDOW_H
