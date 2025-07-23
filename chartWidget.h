// ChartWidget.h
#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE

    class chartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit chartWidget(QWidget *parent = nullptr, const QString &unit = "%", int yMax = 100);

    void addDataPoint(float value); // for live updates

private:
    QLineSeries *lineSeries;
    QLineSeries *baselineSeries;
    QAreaSeries *areaSeries;
    QChart *chart;
    QChartView *chartView;
    QValueAxis *axisX;
    QValueAxis *axisY;
    int xIndex;
};

#endif // CHARTWIDGET_H
