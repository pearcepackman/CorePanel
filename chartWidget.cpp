// ChartWidget.cpp
#include "chartWidget.h"

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

    if (axisY) {
        if (unit == "°C") {
            axisY->setRange(30, yMax); //  start at 30 for temp charts
        } else {
            axisY->setRange(0, yMax);  // default for usage (%)
        }
        axisY->setLabelFormat(QString("%%1%1").arg(unit));
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

