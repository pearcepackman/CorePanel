#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

#include <QPen>
#include <QBrush>
#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//--------------------------------- CPU USAGE LAYOUT ---------------------------------//

    QLineSeries *series = new QLineSeries();
    series->append(0, 12.5);
    series->append(1, 18.3);
    series->append(2, 22.7);
    series->append(3, 30.1);
    series->append(4, 28.6);
    series->append(5, 35.9);
    series->append(6, 41.2);
    series->append(7, 38.0);
    series->append(8, 45.3);
    series->append(9, 51.7);
    series->append(10, 49.8);
    series->append(11, 55.4);
    series->append(12, 60.0);
    series->append(13, 58.2);
    series->append(14, 63.1);
    series->append(15, 67.4);
    series->append(16, 65.0);
    series->append(17, 70.2);
    series->append(18, 74.6);
    series->append(19, 71.3);


    QLineSeries *baseline = new QLineSeries();
    for (int i = 0; i <= 19; ++i) {
        baseline->append(i, 0);
    }
    QAreaSeries *areaSeries = new QAreaSeries(series, baseline);
    areaSeries->setBrush(QColor(100, 200, 255, 100));  // translucent fill
    areaSeries->setPen(QPen(Qt::blue));

    QChart *chart = new QChart();
    chart->addSeries(areaSeries);
    chart->createDefaultAxes();
    chart->legend()->hide();
    chart->setMargins(QMargins(0, 0, 0, 0));
    QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axisY());
    if (axisY) {
        axisY->setRange(0, 100);
        axisY->setLabelFormat("%d%%");
    }


    // ✨ Transparency settings
    chart->setBackgroundVisible(false);
    chart->setBackgroundBrush(Qt::NoBrush);
    chart->setPlotAreaBackgroundVisible(false);
    //->setPlotAreaBackgroundBrush(Qt::NoBrush);
    chart->setMargins(QMargins(0, 0, 0, 0));



    QChartView *chartView = new QChartView(chart);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartView->setRenderHint(QPainter::Antialiasing);

    //chartView->setStyleSheet("background: transparent");
    //chartView->setAttribute(Qt::WA_TranslucentBackground);
    //chartView->setBackgroundBrush(Qt::NoBrush);

    QVBoxLayout *layout = new QVBoxLayout(ui->cpuUsageChart);
    layout->addWidget(chartView);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
//--------------------------------- CPU USAGE LAYOUT ---------------------------------//
//--------------------------------- CPU TEMP LAYOUT ---------------------------------//
    QLineSeries *cpuTemp = new QLineSeries();
    cpuTemp->append(0, 38.2);
    cpuTemp->append(1, 40.1);
    cpuTemp->append(2, 42.5);
    cpuTemp->append(3, 43.0);
    cpuTemp->append(4, 45.3);
    cpuTemp->append(5, 46.7);
    cpuTemp->append(6, 47.2);
    cpuTemp->append(7, 48.9);
    cpuTemp->append(8, 49.6);
    cpuTemp->append(9, 50.1);
    cpuTemp->append(10, 51.8);
    cpuTemp->append(11, 52.4);
    cpuTemp->append(12, 53.6);
    cpuTemp->append(13, 54.2);
    cpuTemp->append(14, 55.1);
    cpuTemp->append(15, 56.5);
    cpuTemp->append(16, 57.0);
    cpuTemp->append(17, 58.3);
    cpuTemp->append(18, 58.9);
    cpuTemp->append(19, 59.4);

    QLineSeries *cpuTempBaseline = new QLineSeries();
    for (int i = 0; i <= 19; ++i) {
        cpuTempBaseline->append(i, 0);
    }

    QAreaSeries *cpuTempAreaSeries = new QAreaSeries(cpuTemp, cpuTempBaseline);
    cpuTempAreaSeries->setBrush(QColor(100, 200, 255, 100));  // translucent fill
    cpuTempAreaSeries->setPen(QPen(Qt::blue));



    QChart *cpuTempChart = new QChart();
    cpuTempChart->addSeries(cpuTempAreaSeries);
    cpuTempChart->createDefaultAxes();
    cpuTempChart->legend()->hide();
    cpuTempChart->setMargins(QMargins(0, 0, 0, 0));

    QValueAxis *cpuTempAxisY = qobject_cast<QValueAxis *>(cpuTempChart->axisY());
    if (cpuTempAxisY) {
        cpuTempAxisY->setRange(0, 100);
        cpuTempAxisY->setLabelFormat("%dC");
    }

    // ✨ Transparency settings
    cpuTempChart->setBackgroundVisible(false);
    cpuTempChart->setBackgroundBrush(Qt::NoBrush);
    cpuTempChart->setPlotAreaBackgroundVisible(false);

    QChartView *cpuTempChartView = new QChartView(cpuTempChart);
    cpuTempChartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    cpuTempChartView->setRenderHint(QPainter::Antialiasing);


    QVBoxLayout *cpuTempLayout = new QVBoxLayout(ui->cpuTempChart);
    cpuTempLayout->addWidget(cpuTempChartView);
    cpuTempLayout->setContentsMargins(0, 0, 0, 0);
    cpuTempLayout->setSpacing(0);




}

MainWindow::~MainWindow()
{
    delete ui;
}
