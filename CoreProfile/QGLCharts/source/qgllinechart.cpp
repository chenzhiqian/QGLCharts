#include "qgllinechart.h"
#include "qgllineset.h"

#include <QDateTime>
#include <QDebug>
#include <QOpenGLFunctions_4_3_Core>
#include "shader/qglshader.h"

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
//     QGLShader::defaultShader()->bindStandardShader(chart);

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

     QOpenGLFunctions_4_3_Core *gl_core =  static_cast<QOpenGLFunctions_4_3_Core *>(QOpenGLContext::currentContext()->versionFunctions());

//     gl_core->glLineWidth(lineSet->mLineWidth);

     double *vertices = new double[7*4*dataLength];    // 每个柱状4个顶点，每个顶点7个属性(3个位置，4个颜色)
     int vertexWriteIndex = 0;

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
                 // vertex location
                 vertices[vertexWriteIndex++] = translateGLPositionX(chart, leftOutPoint.x());
                 vertices[vertexWriteIndex++] = translateGLPositionY(chart, leftOutPoint.y());
                 vertices[vertexWriteIndex++] = 0;
                 // vertex color
                 vertices[vertexWriteIndex++] = lineSet->mLineColor.red() / 255.0f;
                 vertices[vertexWriteIndex++] = lineSet->mLineColor.green() / 255.0f;
                 vertices[vertexWriteIndex++] = lineSet->mLineColor.blue() / 255.0f;
                 vertices[vertexWriteIndex++] = lineSet->mLineColor.alpha() / 255.0f;

                 hasLeftOutPoint = false;
             }

             if (lineSet->mLineMode == LineSplitMode && preEntry != nullptr && fabs(entry->x - preEntry->x) > lineSet->mSplitInterval) {
                drawArray(GL_LINE_STRIP, vertices, vertexWriteIndex/7);
                vertexWriteIndex = 0;
             }

             // vertex location
             vertices[vertexWriteIndex++] = translateGLPositionX(chart, currentPoint.x());
             vertices[vertexWriteIndex++] = translateGLPositionY(chart, currentPoint.y());
             vertices[vertexWriteIndex++] = 0;
             // vertex color
             vertices[vertexWriteIndex++] = lineSet->mLineColor.red() / 255.0f;
             vertices[vertexWriteIndex++] = lineSet->mLineColor.green() / 255.0f;
             vertices[vertexWriteIndex++] = lineSet->mLineColor.blue() / 255.0f;
             vertices[vertexWriteIndex++] = lineSet->mLineColor.alpha() / 255.0f;
         }
         else {
             if (hasLeftOutPoint) {
                 // vertex location
                 vertices[vertexWriteIndex++] = translateGLPositionX(chart, leftOutPoint.x());
                 vertices[vertexWriteIndex++] = translateGLPositionY(chart, leftOutPoint.y());
                 vertices[vertexWriteIndex++] = 0;
                 // vertex color
                 vertices[vertexWriteIndex++] = lineSet->mLineColor.red() / 255.0f;
                 vertices[vertexWriteIndex++] = lineSet->mLineColor.green() / 255.0f;
                 vertices[vertexWriteIndex++] = lineSet->mLineColor.blue() / 255.0f;
                 vertices[vertexWriteIndex++] = lineSet->mLineColor.alpha() / 255.0f;
             }

             if (lineSet->mLineMode == LineSplitMode && preEntry != nullptr && fabs(entry->x - preEntry->x) > lineSet->mSplitInterval) {
                drawArray(GL_LINE_STRIP, vertices, vertexWriteIndex/7);
                vertexWriteIndex = 0;
             }

             // vertex location
             vertices[vertexWriteIndex++] = translateGLPositionX(chart, currentPoint.x());
             vertices[vertexWriteIndex++] = translateGLPositionY(chart, currentPoint.y());
             vertices[vertexWriteIndex++] = 0;
             // vertex color
             vertices[vertexWriteIndex++] = lineSet->mLineColor.red() / 255.0f;
             vertices[vertexWriteIndex++] = lineSet->mLineColor.green() / 255.0f;
             vertices[vertexWriteIndex++] = lineSet->mLineColor.blue() / 255.0f;
             vertices[vertexWriteIndex++] = lineSet->mLineColor.alpha() / 255.0f;

             break;
         }

         preEntry = entry;
     }

     drawArray(GL_LINE_STRIP, vertices, vertexWriteIndex/7);

     delete[] vertices;
 }
