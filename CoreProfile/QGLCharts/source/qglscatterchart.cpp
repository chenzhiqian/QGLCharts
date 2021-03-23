#include "qglscatterchart.h"

#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include "shader/qglshader.h"

QGLScatterChart::QGLScatterChart(QWidget *parent) : QGLBaseChart(parent)
{

}

QGLScatterChart::~QGLScatterChart()
{

}

void QGLScatterChart::drawSet(QGLDataSet *set)
{
    if (set->type() != ScatterType) {
        return;
    }

    drawScatterSet(this, (QGLScatterSet *)set);
}

void QGLScatterChart::drawScatterSet(QGLBaseChart *chart, QGLScatterSet *scatterSet)
{
    QGLShader::defaultShader()->bindPointShader(chart);

    QOpenGLFunctions_4_3_Core *gl_core =  static_cast<QOpenGLFunctions_4_3_Core *>(QOpenGLContext::currentContext()->versionFunctions());

    QVector<ChartDataEntry *> dataList = scatterSet->entryValues();
    int dataLength = dataList.size();
    if (dataLength == 0) {
        return;
    }

    gl_core->glPointSize(scatterSet->mPointSize);

    double xAxisShowMin = chart->xAxis.showMin();
    double xAxisShowMax = chart->xAxis.showMax();

    double leftAxisShowMin = chart->leftAxis.showMin();
    double leftAxisShowMax = chart->leftAxis.showMax();

    double rightAxisShowMin = chart->rightAxis.showMin();
    double rightAxisShowMax = chart->rightAxis.showMax();


    double *vertices = new double[dataLength*7];    // 每个散点为一个顶点, 每个顶点7个属性
    unsigned int vertexWriteIndex = 0;

    for (int i=0;i<dataLength;i++) {
        ChartDataEntry *entry = dataList[i];
        if (entry->x >= xAxisShowMin && entry->x <= xAxisShowMax) {
            if (scatterSet->mAxisDependy == AxisDependyLeft) {
                if (entry->y >= leftAxisShowMin && entry->y <= leftAxisShowMax) {
                    // vertex location
                    vertices[vertexWriteIndex++] = translateGLPositionX(chart, xValuePosition(chart, entry->x));
                    vertices[vertexWriteIndex++] = translateGLPositionY(chart, yValuePosition(chart, entry->y, scatterSet->mAxisDependy));
                    vertices[vertexWriteIndex++] = 0;
                    // vertex color
                    vertices[vertexWriteIndex++] = scatterSet->mPointColor.red() / 255.0f;
                    vertices[vertexWriteIndex++] = scatterSet->mPointColor.green() / 255.0f;
                    vertices[vertexWriteIndex++] = scatterSet->mPointColor.blue() / 255.0f;
                    vertices[vertexWriteIndex++] = scatterSet->mPointColor.alpha() / 255.0f;
                }
            }
            else if (scatterSet->mAxisDependy == AxisDependyRight) {
                if (entry->y >= rightAxisShowMin && entry->y <= rightAxisShowMax) {
                    // vertex location
                    vertices[vertexWriteIndex++] = translateGLPositionX(chart, xValuePosition(chart, entry->x));
                    vertices[vertexWriteIndex++] = translateGLPositionY(chart, yValuePosition(chart, entry->y, scatterSet->mAxisDependy));
                    vertices[vertexWriteIndex++] = 0;
                    // vertex color
                    vertices[vertexWriteIndex++] = scatterSet->mPointColor.red() / 255.0f;
                    vertices[vertexWriteIndex++] = scatterSet->mPointColor.green() / 255.0f;
                    vertices[vertexWriteIndex++] = scatterSet->mPointColor.blue() / 255.0f;
                    vertices[vertexWriteIndex++] = scatterSet->mPointColor.alpha() / 255.0f;
                }
            }
        }
    }

    drawArray(GL_POINTS, vertices, vertexWriteIndex/7);

    delete[] vertices;
}
