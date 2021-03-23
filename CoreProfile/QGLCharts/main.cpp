#include <QApplication>
#include <QDateTime>
#include "source/qgllinechart.h"
#include "source/qglscatterchart.h"
#include <source/qglcombinechart.h>
#include <source/qglhistogramchart.h>

#define TEST_NUMBER (10000)

QString timestampToHHMMSS(double value)
{
   QDateTime time = QDateTime::fromMSecsSinceEpoch(value);
   return  time.toString("HH:mm\nMM-dd");
}

void testLineChart()
{
    QGLLineChart *lineCharts = new QGLLineChart();
    lineCharts->setWindowTitle("QGLLineChart");
    lineCharts->xAxis.setTitleFormatter(timestampToHHMMSS);
    lineCharts->xAxis.setGridColor(Qt::black);
    lineCharts->xAxis.setGridCount(22);
    lineCharts->xAxis.setTitleCount(11);
    lineCharts->xAxis.setTitlePosition(XAxisTitleBottomInner);
    lineCharts->xAxis.setTitleColor(QColor(0x3c3c3c));
    lineCharts->leftAxis.setGridColor(Qt::black);
    lineCharts->leftAxis.setRange(-1000, 1000);
    lineCharts->leftAxis.setGridCount(16);
    lineCharts->leftAxis.setTitleCount(8);
    lineCharts->leftAxis.setTitleColor(QColor(0x3c3c3c));
    lineCharts->setChartPadding(40, 10, 25, 30);
    lineCharts->setZoom(1, 100);

    lineCharts->xAxis.setRange(0, 1000000);
    lineCharts->xAxis.setViewRatio(1);

    QGLLineSet *lineSet = new QGLLineSet(Qt::blue, 1);

    for (int i=0;i<TEST_NUMBER;i++) {
        ChartDataEntry *entry = new ChartDataEntry(i*(1000000.0/TEST_NUMBER), rand()%2000-1000);
        lineSet->addEntry(entry);
    }
    lineCharts->appendData(lineSet);

    lineCharts->show();
}

void testScatterChart()
{
    HighlightStru highlight;
    highlight.min = 1000;
    highlight.max = 2000;
    highlight.color = Qt::yellow;

    QGLScatterChart *scatterCharts = new QGLScatterChart();
    scatterCharts->setBackgroundColor(Qt::gray);
    scatterCharts->setWindowTitle("QGLScatterChart");
    scatterCharts->setChartPadding(40, 10, 25, 30);
    scatterCharts->setZoom(1, 100);

    scatterCharts->xAxis.setGridColor(Qt::blue);
    scatterCharts->xAxis.setGridCount(16);
    scatterCharts->xAxis.setTitleCount(8);
    scatterCharts->xAxis.setTitlePosition(XAxisTitleTop);
    scatterCharts->xAxis.setTitleColor(QColor(0x3c3c3c));
    scatterCharts->xAxis.setCalibrationEnable(true);
    scatterCharts->xAxis.setMajorCalibrationCount(16);
    scatterCharts->xAxis.setMinorCalibrationCount(5);
    scatterCharts->xAxis.setCalibrationHeight(10);
    scatterCharts->xAxis.setGridWidth(1);

    scatterCharts->leftAxis.setGridColor(Qt::black);
    scatterCharts->leftAxis.setRange(0, 10000);
    scatterCharts->leftAxis.setGridCount(16);
    scatterCharts->leftAxis.setTitleCount(8);
    scatterCharts->leftAxis.setTitleColor(QColor(0x3c3c3c));
    scatterCharts->leftAxis.setCalibrationEnable(true);
    scatterCharts->leftAxis.setMajorCalibrationCount(16);
    scatterCharts->leftAxis.setMinorCalibrationCount(5);
    scatterCharts->leftAxis.setCalibrationHeight(10);
    scatterCharts->leftAxis.setGridColor(Qt::red);


    scatterCharts->setEnableRightAxis(true);
     scatterCharts->rightAxis.setRange(0, 10000);
     scatterCharts->rightAxis.setGridCount(16);
     scatterCharts->rightAxis.setTitleCount(8);
     scatterCharts->rightAxis.setTitleColor(QColor(0x3c3c3c));
     scatterCharts->rightAxis.setCalibrationEnable(true);
     scatterCharts->rightAxis.setMajorCalibrationCount(16);
     scatterCharts->rightAxis.setMinorCalibrationCount(2);
     scatterCharts->rightAxis.setCalibrationHeight(10);
     scatterCharts->rightAxis.setGridColor(Qt::yellow);

     scatterCharts->setChartPadding(50, 30, 50, 30);
    scatterCharts->xAxis.appendHighlight(highlight);
    highlight.min = 500;
    highlight.max = 800;
    highlight.color = Qt::green;
    scatterCharts->xAxis.appendHighlight(highlight);

    highlight.color = Qt::blue;
    scatterCharts->leftAxis.appendHighlight(highlight);

    scatterCharts->xAxis.setRange(0, 10000);
    scatterCharts->xAxis.setViewRatio(1);

    scatterCharts->setEnableSelected(true, 1, 10, Qt::yellow);

    QGLScatterSet *scatterSet = new QGLScatterSet(Qt::blue, 4);

    for (int i=0;i<TEST_NUMBER;i++) {
        ChartDataEntry *entry = new ChartDataEntry(rand()%10000, rand()%10000);
        scatterSet->addEntry(entry);
    }
    scatterCharts->appendData(scatterSet);

    scatterCharts->show();
}

void testHistogramChart()
{
    QGLHistogramChart *histogramCharts = new QGLHistogramChart();
    histogramCharts->setWindowTitle("QGLHistogramChart");
    histogramCharts->setBackgroundColor(Qt::lightGray);
    histogramCharts->setChartPadding(60, 20, 60, 40);
    histogramCharts->setZoom(1, 100);
    histogramCharts->xAxis.setGridColor(Qt::blue);
    histogramCharts->xAxis.setGridCount(16);
    histogramCharts->xAxis.setTitleCount(8);
    histogramCharts->xAxis.setTitlePosition(XAxisTitleBottom);
    histogramCharts->xAxis.setTitleColor(QColor(0x3c3c3c));
    histogramCharts->xAxis.setRange(0, 2000);
    histogramCharts->xAxis.setViewRatio(1);
    histogramCharts->xAxis.setCalibrationEnable(true);
    histogramCharts->xAxis.setMajorCalibrationCount(16);
    histogramCharts->xAxis.setMinorCalibrationCount(5);
    histogramCharts->xAxis.setCalibrationHeight(10);

    histogramCharts->leftAxis.setGridColor(Qt::black);
    histogramCharts->leftAxis.setRange(0, 10000);
    histogramCharts->leftAxis.setGridCount(16);
    histogramCharts->leftAxis.setTitlePosition(YAxisBaselineLeft);
    histogramCharts->leftAxis.setTitleCount(8);
    histogramCharts->leftAxis.setTitleColor(QColor(0x3c3c3c));
    histogramCharts->leftAxis.setCalibrationEnable(true);
    histogramCharts->leftAxis.setMajorCalibrationCount(16);
    histogramCharts->leftAxis.setMinorCalibrationCount(5);
    histogramCharts->leftAxis.setCalibrationHeight(10);
    histogramCharts->leftAxis.setGridColor(Qt::red);

    histogramCharts->setEnableRightAxis(true);
    histogramCharts->rightAxis.setTitlePosition(YAxisBaselineRight);
    histogramCharts->rightAxis.setGridColor(Qt::black);
    histogramCharts->rightAxis.setRange(0, 5000);
    histogramCharts->rightAxis.setGridCount(16);
    histogramCharts->rightAxis.setTitleCount(8);
    histogramCharts->rightAxis.setTitleColor(QColor(0x3c3c3c));
    histogramCharts->rightAxis.setCalibrationEnable(true);
    histogramCharts->rightAxis.setMajorCalibrationCount(16);
    histogramCharts->rightAxis.setMinorCalibrationCount(2);
    histogramCharts->rightAxis.setCalibrationHeight(10);
    histogramCharts->rightAxis.setGridColor(Qt::red);

    QGLHistogramSet *histogramSet = new QGLHistogramSet(Qt::gray, Qt::cyan, 10000.0/TEST_NUMBER);

    for (int i=0;i<TEST_NUMBER;i++) {
        ChartDataEntry *entry = new HistogramDataEntry(i*(10000.0/TEST_NUMBER), 0, rand()%10000);
        histogramSet->addEntry(entry);
    }
    histogramCharts->appendData(histogramSet);

    histogramCharts->show();
}

void testCombineChart()
{
    QGLCombineChart *combineCharts = new QGLCombineChart();
    combineCharts->setWindowTitle("QGLCombineChart");
    combineCharts->setBackgroundColor(Qt::darkGray);
    combineCharts->setChartPadding(40, 30, 25, 30);     // 图表绘图区填充
    combineCharts->setZoom(1, 100);         // 最大缩放倍率

    combineCharts->xAxis.setGridColor(Qt::blue);
    combineCharts->xAxis.setGridCount(16);    // 网格个数
    combineCharts->xAxis.setTitleCount(8);        // 标题个数
    combineCharts->xAxis.setTitlePosition(XAxisTitleTop); // 标题显示位置
    combineCharts->xAxis.setTitleColor(QColor(0x3c3c3c)); // 标题颜色
    combineCharts->xAxis.setRange(0, 2000);     // 设置x轴数值范围
    combineCharts->xAxis.setViewRatio(1);          // 设置x轴每页显示占比
    combineCharts->xAxis.setCalibrationEnable(true);  // 标尺
    combineCharts->xAxis.setMajorCalibrationCount(16);  // 主标尺个数
    combineCharts->xAxis.setMinorCalibrationCount(5); // 子标尺个数
    combineCharts->xAxis.setCalibrationHeight(10);    // 标尺高度

    combineCharts->leftAxis.setGridColor(Qt::black);
    combineCharts->leftAxis.setRange(0, 10000);
    combineCharts->leftAxis.setGridCount(16);
    combineCharts->leftAxis.setTitleCount(8);
    combineCharts->leftAxis.setTitleColor(QColor(0x3c3c3c));
    combineCharts->leftAxis.setCalibrationEnable(true);
    combineCharts->leftAxis.setMajorCalibrationCount(16);
    combineCharts->leftAxis.setMinorCalibrationCount(5);
    combineCharts->leftAxis.setCalibrationHeight(10);
    combineCharts->leftAxis.setGridColor(Qt::red);

    QGLHistogramSet *histogramSet = new QGLHistogramSet(Qt::gray, Qt::blue, 10000.0/TEST_NUMBER);

    for (int i=0;i<TEST_NUMBER;i++) {
        ChartDataEntry *entry = new HistogramDataEntry(i*(10000.0/TEST_NUMBER), 0, rand()%10000);
        histogramSet->addEntry(entry);
    }
    combineCharts->appendData(histogramSet);


    QGLScatterSet *scatterSet = new QGLScatterSet(Qt::red, 4);

    for (int i=0;i<TEST_NUMBER;i++) {
        ChartDataEntry *entry = new ChartDataEntry(rand()%10000, rand()%10000);
        scatterSet->addEntry(entry);
    }
    combineCharts->appendData(scatterSet);

    QGLLineSet *lineSet = new QGLLineSet(Qt::yellow, 1);

    for (int i=0;i<TEST_NUMBER;i++) {
        ChartDataEntry *entry = new ChartDataEntry(i*(2000.0/TEST_NUMBER), rand()%2000);
        lineSet->addEntry(entry);
    }
    combineCharts->appendData(lineSet);

    combineCharts->show();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    testLineChart();
    testScatterChart();
    testHistogramChart();
    testCombineChart();

    return a.exec();
}


