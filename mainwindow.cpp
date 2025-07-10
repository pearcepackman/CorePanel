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
    cpuTemp->append(0,50);
    cpuTemp->append(1,46);
    cpuTemp->append(2,58);
    cpuTemp->append(3,60);

    QChart *cpuTempChart = new QChart();
    cpuTempChart->addSeries(cpuTemp);
    cpuTempChart->createDefaultAxes();
    cpuTempChart->legend()->hide();

    QChartView *cpuTempChartView = new QChartView(cpuTempChart);
    cpuTempChartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    cpuTempChartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *cpuTempLayout = new QVBoxLayout(ui->cpuTemp_2);
    cpuTempLayout->addWidget(cpuTempChartView);
    cpuTempLayout->setContentsMargins(0, 0, 0, 0);
    cpuTempLayout->setSpacing(0);



}

MainWindow::~MainWindow()
{
    delete ui;
}
