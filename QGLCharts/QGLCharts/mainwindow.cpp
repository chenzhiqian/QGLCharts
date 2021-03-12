#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "source/qglscatterchart.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    HighlightStru highlight;
    highlight.min = 1000;
    highlight.max = 2000;
    highlight.color = Qt::yellow;

    QGLScatterChart *scatterCharts = new QGLScatterChart();
    scatterCharts->xAxis.setGridColor(Qt::blue);
    scatterCharts->xAxis.setGridCount(16);
    scatterCharts->xAxis.setTitleCount(8);
    scatterCharts->xAxis.setTitlePosition(XAxisTitleTop);
    scatterCharts->xAxis.setTitleColor(QColor(0x3c3c3c));
    scatterCharts->leftAxis.setGridColor(Qt::black);
    scatterCharts->leftAxis.setRange(0, 10000);
    scatterCharts->leftAxis.setGridCount(16);
    scatterCharts->leftAxis.setTitleCount(8);
    scatterCharts->leftAxis.setTitleColor(QColor(0x3c3c3c));
    scatterCharts->setChartPadding(40, 10, 25, 30);
    scatterCharts->setZoom(1, 100);
    scatterCharts->xAxis.setCalibrationEnable(true);
    scatterCharts->xAxis.setMajorCalibrationCount(16);
    scatterCharts->xAxis.setMinorCalibrationCount(5);
    scatterCharts->xAxis.setCalibrationHeight(10);

    scatterCharts->leftAxis.setCalibrationEnable(true);
    scatterCharts->leftAxis.setMajorCalibrationCount(16);
    scatterCharts->leftAxis.setMinorCalibrationCount(5);
    scatterCharts->leftAxis.setCalibrationHeight(10);
    scatterCharts->leftAxis.setGridColor(Qt::red);

    scatterCharts->setEnableRightAxis(true);
     scatterCharts->rightAxis.setGridColor(Qt::black);
     scatterCharts->rightAxis.setRange(0, 10000);
     scatterCharts->rightAxis.setGridCount(16);
     scatterCharts->rightAxis.setTitleCount(8);
     scatterCharts->rightAxis.setTitleColor(QColor(0x3c3c3c));
     scatterCharts->rightAxis.setCalibrationEnable(true);
     scatterCharts->rightAxis.setMajorCalibrationCount(16);
     scatterCharts->rightAxis.setMinorCalibrationCount(2);
     scatterCharts->rightAxis.setCalibrationHeight(10);
     scatterCharts->rightAxis.setGridColor(Qt::red);

     scatterCharts->setChartPadding(50, 30, 50, 30);
    scatterCharts->xAxis.appendHighlight(highlight);

    highlight.color = Qt::blue;
    scatterCharts->leftAxis.appendHighlight(highlight);

    scatterCharts->xAxis.setRange(0, 10000);
    scatterCharts->xAxis.setViewRatio(1);

    scatterCharts->setEnableSelected(true, 1, 10, Qt::yellow);

    QGLScatterSet *scatterSet = new QGLScatterSet(Qt::red, 4);

    for (int i=0;i<100;i++) {
        ChartDataEntry *entry = new ChartDataEntry(rand()%10000, rand()%10000);
        scatterSet->addEntry(entry);
    }
    scatterCharts->appendData(scatterSet);

    this->setCentralWidget(scatterCharts);
}

MainWindow::~MainWindow()
{

}

