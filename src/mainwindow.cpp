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

    //Using HardwareReader in the directory to open when this app opens, then read the temps/usages
    hardwareReaderProcess = new QProcess(this);
    QString exePath = QCoreApplication::applicationDirPath() + "/HardwareReader.exe";
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
                
                //Extracting the value of the lines coming from C# program
                if (line.startsWith("CPU_USAGE=")) {
                    float val = line.section('=', 1).toFloat();
                    //Update the UI to set the text and add data points
                    ui->cpuUsageOutput->setText(QString::number(val, 'f', 1) + "%");
                    cpuUsageChartWidget->addDataPoint(val);
                }

                if (line.startsWith("CPU_TEMP=")) {
                    float val = line.section('=', 1).toFloat();
                    ui->cpuTempOutput->setText(QString::number(val, 'f', 1) + "C");
                    cpuTempChartWidget->addDataPoint(val);
                }

                if (line.startsWith("GPU_USAGE=")) {
                    float val = line.section('=', 1).toFloat();
                    ui->gpuUsageOutput->setText(QString::number(val, 'f', 1) + "%");
                    gpuUsageChartWidget->addDataPoint(val);
                }
                if (line.startsWith("GPU_TEMP=")) {
                    float val = line.section('=', 1).toFloat();
                    ui->gpuTempOutput->setText(QString::number(val, 'f', 1) + "C");
                    gpuTempChartWidget->addDataPoint(val);
                }
                if (line.startsWith("DISK_USAGE=")) {
                    float val = line.section('=', 1).toFloat();
                    ui->diskUsageOutput->setText(QString::number(val, 'f', 1) + "%");
                    diskUsageChartWidget->addDataPoint(val);
                }
                if (line.startsWith("RAM_USAGE=")) {
                    float val = line.section('=', 1).toFloat();
                    ui->ramUsageOutput->setText(QString::number(val, 'f', 1) + "%");
                    ramUsageChartWidget->addDataPoint(val);
                }

            }
        }
    });










//--------------------------------- CPU USAGE LAYOUT ---------------------------------//
    //Adding widget to chart UI chart 
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

//--------------------------------- CPU TEMP LAYOUT ---------------------------------//

//--------------------------------- GPU USAGE LAYOUT ---------------------------------//

    gpuUsageChartWidget = new chartWidget(this, "%", 100);
    QVBoxLayout *gpuUsageLayout = new QVBoxLayout(ui->gpuUsageChart);
    gpuUsageLayout->addWidget(gpuUsageChartWidget);
    gpuUsageLayout->setContentsMargins(0,0,0,0);
    gpuUsageLayout->setSpacing(0);

//--------------------------------- GPU USAGE LAYOUT ---------------------------------//

//--------------------------------- GPU TEMP LAYOUT ---------------------------------//

    gpuTempChartWidget = new chartWidget(this, "C", 100);
    QVBoxLayout *gpuTempLayout = new QVBoxLayout(ui->gpuTempChart);
    gpuTempLayout->addWidget(gpuTempChartWidget);
    gpuTempLayout->setContentsMargins(0,0,0,0);
    gpuTempLayout->setSpacing(0);

//--------------------------------- GPU TEMP LAYOUT ---------------------------------//

//--------------------------------- DISK USAGE LAYOUT ---------------------------------//

    diskUsageChartWidget = new chartWidget(this, "%", 100);
    QVBoxLayout *diskUsageLayout = new QVBoxLayout(ui->diskUsageChart);
    diskUsageLayout->addWidget(diskUsageChartWidget);
    diskUsageLayout->setContentsMargins(0,0,0,0);
    diskUsageLayout->setSpacing(0);

//--------------------------------- DISK USAGE LAYOUT ---------------------------------//

//--------------------------------- RAM USAGE LAYOUT ---------------------------------//

    ramUsageChartWidget = new chartWidget(this, "%", 100);
    QVBoxLayout *ramUsageLayout = new QVBoxLayout(ui->ramUsageChart);
    ramUsageLayout->addWidget(ramUsageChartWidget);
    ramUsageLayout->setContentsMargins(0,0,0,0);
    ramUsageLayout->setSpacing(0);

//--------------------------------- RAM USAGE LAYOUT ---------------------------------//




}

MainWindow::~MainWindow()
{   
    //Deconstructor, will kill HardwareReader in the background when app closes
    if (hardwareReaderProcess && hardwareReaderProcess->state() != QProcess::NotRunning) {
        hardwareReaderProcess->terminate(); // Send gentle close signal
        if (!hardwareReaderProcess->waitForFinished(1000)) {
            hardwareReaderProcess->kill(); // Force kill if still hanging
            hardwareReaderProcess->waitForFinished();
        }
    }

    delete ui;
}
