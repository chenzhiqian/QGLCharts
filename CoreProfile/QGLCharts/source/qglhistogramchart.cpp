#include "qglhistogramchart.h"

#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include "shader/qglshader.h"

QGLHistogramChart::QGLHistogramChart(QWidget *parent) : QGLBaseChart(parent)
{

}

QGLHistogramChart::~QGLHistogramChart()
{

}

void QGLHistogramChart::drawSet(QGLDataSet *set)
{
    if (set->type() != HistogramType) {
        return;
    }

    drawHistogramSet(this, (QGLHistogramSet *)set);
}

void QGLHistogramChart::drawHistogramSet(QGLBaseChart *chart, QGLHistogramSet *histogramSet)
{
    QGLShader::defaultShader()->bindStandardShader(chart);

    QOpenGLFunctions_4_3_Core *gl_core =  static_cast<QOpenGLFunctions_4_3_Core *>(QOpenGLContext::currentContext()->versionFunctions());

    QVector<ChartDataEntry *> dataList = histogramSet->entryValues();
    int dataLength = dataList.size();
    if (dataLength == 0) {
        return;
    }

    double xAxisShowMin = chart->xAxis.showMin();
    double xAxisShowMax = chart->xAxis.showMax();

    double *vertices = new double[7*4*dataLength];    // 每个柱状4个顶点，每个顶点7个属性(3个位置，4个颜色)
    unsigned int *fillElements = nullptr;
    if (histogramSet->mFillEnable) {
        fillElements = new unsigned int[dataLength*6];     // 每个柱状6个索引点
    }
    unsigned int *borderElements = nullptr;
    if (histogramSet->mBorderEnable) {
        borderElements = new unsigned int[dataLength*8]; // 每个柱状框8个索引点
    }
    int vertexWriteIndex = 0;
    int fillElementWriteIndex = 0;
    int borderElementWriteIndex = 0;

    int vertexCount = 0;

    for (int i=0;i<dataLength;i++) {
        HistogramDataEntry *entry = (HistogramDataEntry *)dataList[i];
        if (entry->x + histogramSet->mDataSpan < xAxisShowMin || entry->x > xAxisShowMax) {
            continue;
        }

        // vertex location
        vertices[vertexWriteIndex++] = translateGLPositionX(chart, xValuePosition(chart, entry->x));
        vertices[vertexWriteIndex++] = translateGLPositionY(chart, yValuePosition(chart, entry->yStart, histogramSet->mAxisDependy));
        vertices[vertexWriteIndex++] = 0;
        // vertex color
        vertices[vertexWriteIndex++] = histogramSet->mFillColor.red() / 255.0f;
        vertices[vertexWriteIndex++] = histogramSet->mFillColor.green() / 255.0f;
        vertices[vertexWriteIndex++] = histogramSet->mFillColor.blue() / 255.0f;
        vertices[vertexWriteIndex++] = histogramSet->mFillColor.alpha() / 255.0f;

        // vertex location
        vertices[vertexWriteIndex++] = translateGLPositionX(chart, xValuePosition(chart, entry->x+histogramSet->mDataSpan));
        vertices[vertexWriteIndex++] = translateGLPositionY(chart, yValuePosition(chart, entry->yStart, histogramSet->mAxisDependy));
        vertices[vertexWriteIndex++] = 0;
        // vertex color
        vertices[vertexWriteIndex++] = histogramSet->mFillColor.red() / 255.0f;
        vertices[vertexWriteIndex++] = histogramSet->mFillColor.green() / 255.0f;
        vertices[vertexWriteIndex++] = histogramSet->mFillColor.blue() / 255.0f;
        vertices[vertexWriteIndex++] = histogramSet->mFillColor.alpha() / 255.0f;

        // vertex location
        vertices[vertexWriteIndex++] = translateGLPositionX(chart, xValuePosition(chart, entry->x+histogramSet->mDataSpan));
        vertices[vertexWriteIndex++] = translateGLPositionY(chart, yValuePosition(chart, entry->yEnd, histogramSet->mAxisDependy));
        vertices[vertexWriteIndex++] = 0;
        // vertex color
        vertices[vertexWriteIndex++] = histogramSet->mFillColor.red() / 255.0f;
        vertices[vertexWriteIndex++] = histogramSet->mFillColor.green() / 255.0f;
        vertices[vertexWriteIndex++] = histogramSet->mFillColor.blue() / 255.0f;
        vertices[vertexWriteIndex++] = histogramSet->mFillColor.alpha() / 255.0f;

        // vertex location
        vertices[vertexWriteIndex++] = translateGLPositionX(chart, xValuePosition(chart, entry->x));
        vertices[vertexWriteIndex++] = translateGLPositionY(chart, yValuePosition(chart, entry->yEnd, histogramSet->mAxisDependy));
        vertices[vertexWriteIndex++] = 0;
        // vertex color
        vertices[vertexWriteIndex++] = histogramSet->mFillColor.red() / 255.0f;
        vertices[vertexWriteIndex++] = histogramSet->mFillColor.green() / 255.0f;
        vertices[vertexWriteIndex++] = histogramSet->mFillColor.blue() / 255.0f;
        vertices[vertexWriteIndex++] = histogramSet->mFillColor.alpha() / 255.0f;

        vertexCount += 4;

        if (histogramSet->mFillEnable) {
            fillElements[fillElementWriteIndex++] = vertexCount - 1;
            fillElements[fillElementWriteIndex++] = vertexCount - 3;
            fillElements[fillElementWriteIndex++] = vertexCount - 4;
            fillElements[fillElementWriteIndex++] = vertexCount - 3;
            fillElements[fillElementWriteIndex++] = vertexCount - 2;
            fillElements[fillElementWriteIndex++] = vertexCount - 1;
        }

        if (histogramSet->mBorderEnable) {
            borderElements[borderElementWriteIndex++] = vertexCount - 4;
            borderElements[borderElementWriteIndex++] = vertexCount - 3;
            borderElements[borderElementWriteIndex++] = vertexCount - 3;
            borderElements[borderElementWriteIndex++] = vertexCount - 2;
            borderElements[borderElementWriteIndex++] = vertexCount - 2;
            borderElements[borderElementWriteIndex++] = vertexCount - 1;
            borderElements[borderElementWriteIndex++] = vertexCount - 1;
            borderElements[borderElementWriteIndex++] = vertexCount - 4;
        }
    }

    if (histogramSet->mFillEnable) {
        gl_core->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawElement(GL_TRIANGLES, vertices, vertexCount, fillElements, vertexCount/4*6);
    }

    if (histogramSet->mBorderEnable) {
        for (int i=0;i<vertexCount;i++) {
            vertices[i*7+3] = histogramSet->mBorderColor.red() / 255.0f;
            vertices[i*7+4] = histogramSet->mBorderColor.green() / 255.0f;
            vertices[i*7+5] = histogramSet->mBorderColor.blue() / 255.0f;
            vertices[i*7+6] = histogramSet->mBorderColor.alpha() / 255.0f;
        }

        gl_core->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        drawElement(GL_LINES, vertices, vertexCount, borderElements, vertexCount/4*8);

        gl_core->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    delete[] vertices;
    if (fillElements != nullptr) {
        delete[] fillElements;
    }
    if (borderElements != nullptr) {
        delete[] borderElements;
    }
}
