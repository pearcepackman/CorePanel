// ChartWidget.cpp
#include "chartWidget.h"
#include <QLabel>
#include <QHBoxLayout>

//Creating chart with filled in area under the line
chartWidget::chartWidget(QWidget *parent, const QString &unit, int yMax)
    : QWidget(parent), xIndex(0)
{
    //line is top of the area, where baseline is at the bottom for the shading
    lineSeries = new QLineSeries();
    baselineSeries = new QLineSeries();

    //This is the area below the line getting filled in
    areaSeries = new QAreaSeries(lineSeries, baselineSeries);
    areaSeries->setBrush(QColor(100, 200, 255, 100));
    areaSeries->setPen(QPen(Qt::blue));

    //Creating chart, adding series, hides legend, styling, etc.
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

    //X axis has no labels
    if (axisX) {
        axisX->setLabelsVisible(false);     // hides numbers
        axisX->setTickCount(0);             // hides ticks
        axisX->setLineVisible(false);       // hides axis line
    }

    //Defining range + styling for celsius
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
            // Defining range + styling for usage
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

    // Lets QChart be shown in a widget, expanded in size and tightly fit UI
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
    // If there's more than 20 points, remove oldest at [0]
    const int MAX_POINTS = 20;
    if (lineSeries->count() >= MAX_POINTS) {
        lineSeries->remove(0);
        baselineSeries->remove(0);
    }
    // Stick a new point onto the graph
    lineSeries->append(xIndex, value);
    baselineSeries->append(xIndex, 0);

    // Scroll X axis so it shows the last 20 points
    if (axisX) {
        axisX->setRange(xIndex - MAX_POINTS + 1, xIndex);
    }
    // Moves x coord for next point to be placed
    xIndex++;
}

