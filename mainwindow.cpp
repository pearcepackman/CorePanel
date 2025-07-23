#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QPen>
#include <QBrush>
#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <QStandardPaths>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "chartWidget.h"
using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Executable path is just hardcoded right now, will update this
    hardwareReaderProcess = new QProcess(this);
    QString exePath = "C:/Users/table/Desktop/Programming/CorePanel/HardwareReader/bin/Release/net9.0/win-x64/publish/HardwareReader.exe";
    if (!QFile::exists(exePath)) {
        qDebug() << "ERROR: Executable not found at" << exePath;
    } else {
        //Starts the HardwareMonitor for capturing hardware data
        hardwareReaderProcess->setProgram(exePath);
        hardwareReaderProcess->start();
    }


    // Connecting the process to readyRead for easy output
    connect(hardwareReaderProcess, &QProcess::readyReadStandardOutput, this, [=]() {
        QByteArray data = hardwareReaderProcess->readAllStandardOutput();
        QStringList lines = QString(data).split('\n', Qt::SkipEmptyParts);
        //Getting the lines of output from my C# program
        for (const QString &rawline : lines) {
            QString line = rawline.trimmed();
            if (line.contains('=')) {
                qDebug() << "Metric:" << line;

                if (line.startsWith("CPU_USAGE=")) {
                    float val = line.section('=', 1).toFloat();
                    ui->cpuUsageOutput->setText(QString::number(val, 'f', 1) + "%");
                    cpuUsageChartWidget->addDataPoint(val);
                }

                if (line.startsWith("CPU_TEMP=")) {
                    float val = line.section('=', 1).toFloat();
                    ui->cpuTempOutput->setText(QString::number(val, 'f', 1) + "C");
                    cpuTempChartWidget->addDataPoint(val);
                }
                //Working on this one, UI needs widget for this
                /*if (line.startsWith("RAM_USAGE=")) {
                    QString val = line.section('=', 1);
                    ui->ramUsageOutput->setText(val + "%");
                }
*/
                if (line.startsWith("GPU_USAGE=")) {
                    QString val = line.section('=', 1);
                    ui->gpuUsageOutput->setText(val + "%");
                }
                if (line.startsWith("GPU_TEMP=")) {
                    QString val = line.section('=', 1);
                    ui->gpuTempOutput->setText(val + "C");
                }
            }
        }
    });










//--------------------------------- CPU USAGE LAYOUT ---------------------------------//

    cpuUsageChartWidget = new chartWidget(this, "%", 100);
    QVBoxLayout *cpuLayout = new QVBoxLayout(ui->cpuUsageChart);
    cpuLayout->addWidget(cpuUsageChartWidget);
    cpuLayout->setContentsMargins(0,0,0,0);
    cpuLayout->setSpacing(0);


//--------------------------------- CPU USAGE LAYOUT ---------------------------------//
//--------------------------------- CPU TEMP LAYOUT ---------------------------------//

    cpuTempChartWidget = new chartWidget(this, "C", 100);
    QVBoxLayout *cpuTempLayout = new QVBoxLayout(ui->cpuTempChart);
    cpuTempLayout->addWidget(cpuTempChartWidget);
    cpuTempLayout->setContentsMargins(0,0,0,0);
    cpuTempLayout->setSpacing(0);




}

MainWindow::~MainWindow()
{
    if (hardwareReaderProcess && hardwareReaderProcess->state() != QProcess::NotRunning) {
        hardwareReaderProcess->terminate(); // Send gentle close signal
        if (!hardwareReaderProcess->waitForFinished(1000)) {
            hardwareReaderProcess->kill(); // Force kill if still hanging
            hardwareReaderProcess->waitForFinished();
        }
    }

    delete ui;
}
