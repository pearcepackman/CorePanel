// ChartWidget.cpp
#include "chartWidget.h"
#include <QLabel>
#include <QHBoxLayout>

chartWidget::chartWidget(QWidget *parent, const QString &unit, int yMax)
    : QWidget(parent), xIndex(0)
{
    lineSeries = new QLineSeries();
    baselineSeries = new QLineSeries();

    areaSeries = new QAreaSeries(lineSeries, baselineSeries);
    areaSeries->setBrush(QColor(100, 200, 255, 100));
    areaSeries->setPen(QPen(Qt::blue));

    chart = new QChart();
    chart->addSeries(areaSeries);
    chart->legend()->hide();
    chart->createDefaultAxes();
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundVisible(false);
    chart->setBackgroundBrush(Qt::NoBrush);
    chart->setPlotAreaBackgroundVisible(false);

    axisX = qobject_cast<QValueAxis *>(chart->axisX());
    axisY = qobject_cast<QValueAxis *>(chart->axisY());

    if (axisX) {
        axisX->setLabelsVisible(false);     // hides numbers
        axisX->setTickCount(0);             // hides ticks
        axisX->setLineVisible(false);       // hides axis line
    }

    if (axisY) {
        if (unit == "C") {
            // Temp chart: hide everything
            axisY->setRange(30, 105);
            axisY->setLabelsVisible(true);
            axisY->setTickCount(5);
            axisY->setLineVisible(false);
            axisY->setGridLineVisible(true);
            axisY->setLabelFormat("%d");
        } else {
            // Usage chart: show labeled ticks
            axisY->setRange(0, yMax);
            axisY->setTickCount(5);
            axisY->setLabelsVisible(true);

            if (unit == "%") {
                axisY->setLabelFormat("%d%%");
            } else {
                axisY->setLabelFormat("%d");
            }
        }
    }



    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);


}

void chartWidget::addDataPoint(float value)
{
    const int MAX_POINTS = 20;

    if (lineSeries->count() >= MAX_POINTS) {
        lineSeries->remove(0);
        baselineSeries->remove(0);
    }

    lineSeries->append(xIndex, value);
    baselineSeries->append(xIndex, 0);

    // Scroll the X-axis to always show the last MAX_POINTS
    if (axisX) {
        axisX->setRange(xIndex - MAX_POINTS + 1, xIndex);
    }

    xIndex++;
}

