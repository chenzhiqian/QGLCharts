#include "qgllinechart.h"
#include "qgllineset.h"

#include <QDateTime>
#include <QDebug>

QGLLineChart::QGLLineChart(QWidget *parent) : QGLBaseChart(parent)
{

}

QGLLineChart::~QGLLineChart()
{

}

void QGLLineChart::drawSet(QGLDataSet *set)
{
    if (set->type() != LineType) {
        return;
    }

    drawLineSet(this, (QGLLineSet *)set);
}

 void QGLLineChart::drawLineSet(QGLBaseChart *chart, QGLLineSet *lineSet)
 {
     QVector<ChartDataEntry *> dataList = lineSet->entryValues();
     int dataLength = dataList.size();
     if (dataLength == 0) {
         return;
     }

    double xAxisShowMin = chart->xAxis.showMin();
    double xAxisShowMax = chart->xAxis.showMax();
     // TODO: 当前set数据默认升序排列，需要处理无序场景
     ChartDataEntry *firstEntry = dataList[0];
     ChartDataEntry *lastEntry = dataList[dataLength-1];
     if (firstEntry->x > xAxisShowMax || lastEntry->x < xAxisShowMin) {
         return;
     }

     glLineWidth(lineSet->mLineWidth);
     glColor3f(lineSet->mLineColor.red() / 255.0f, lineSet->mLineColor.green() / 255.0f, lineSet->mLineColor.blue() / 255.0f);

     glBegin(GL_LINE_STRIP);

     QPointF leftOutPoint;
     bool hasLeftOutPoint = false;
     ChartDataEntry *preEntry = nullptr;

     for (int i=0;i<dataLength;i++) {
         ChartDataEntry *entry = dataList[i];
         QPointF currentPoint = QPointF(xValuePosition(chart, entry->x), yValuePosition(chart, entry->y, lineSet->mAxisDependy));

         if (entry->x < xAxisShowMin) {
             hasLeftOutPoint = true;
             leftOutPoint = currentPoint;
         }
         else if (entry->x < xAxisShowMax) {
             if (hasLeftOutPoint) {
                 glVertex3f(translateGLPositionX(chart, leftOutPoint.x()), translateGLPositionY(chart, leftOutPoint.y()), 0);
                 hasLeftOutPoint = false;
             }

             if (lineSet->mLineMode == LineSplitMode && preEntry != nullptr && fabs(entry->x - preEntry->x) > lineSet->mSplitInterval) {
                glEnd();
                glLineWidth(lineSet->mLineWidth);
                glColor3f(lineSet->mLineColor.red() / 255.0f, lineSet->mLineColor.green() / 255.0f, lineSet->mLineColor.blue() / 255.0f);
                glBegin(GL_LINE_STRIP);
             }

             glVertex3f(translateGLPositionX(chart, currentPoint.x()), translateGLPositionY(chart, currentPoint.y()), 0);
         }
         else {
             if (hasLeftOutPoint) {
                 glVertex3f(translateGLPositionX(chart, leftOutPoint.x()), translateGLPositionY(chart, leftOutPoint.y()), 0);
             }

             if (lineSet->mLineMode == LineSplitMode && preEntry != nullptr && fabs(entry->x - preEntry->x) > lineSet->mSplitInterval) {
                glEnd();
                glLineWidth(lineSet->mLineWidth);
                glColor3f(lineSet->mLineColor.red() / 255.0f, lineSet->mLineColor.green() / 255.0f, lineSet->mLineColor.blue() / 255.0f);
                glBegin(GL_LINE_STRIP);
             }

             glVertex3f(translateGLPositionX(chart, currentPoint.x()), translateGLPositionY(chart, currentPoint.y()), 0);

             break;
         }

         preEntry = entry;
     }

     glEnd();
 }
