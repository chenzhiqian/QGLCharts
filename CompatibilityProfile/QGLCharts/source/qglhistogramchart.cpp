#include "qglhistogramchart.h"

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

    glDisable(GL_MULTISAMPLE);
    drawHistogramSet(this, (QGLHistogramSet *)set);
    glEnable(GL_MULTISAMPLE);
}

void QGLHistogramChart::drawHistogramSet(QGLBaseChart *chart, QGLHistogramSet *histogramSet)
{
    QVector<ChartDataEntry *> dataList = histogramSet->entryValues();
    int dataLength = dataList.size();
    if (dataLength == 0) {
        return;
    }

    double xAxisShowMin = chart->xAxis.showMin();
    double xAxisShowMax = chart->xAxis.showMax();

    for (int i=0;i<dataLength;i++) {
        HistogramDataEntry *entry = (HistogramDataEntry *)dataList[i];
        if (entry->x + histogramSet->mDataSpan < xAxisShowMin || entry->x > xAxisShowMax) {
            continue;
        }

        QVector<QPointF> vertex;
        vertex.push_back(QPointF(translateGLPositionX(chart, xValuePosition(chart, entry->x)), translateGLPositionY(chart, yValuePosition(chart, entry->yStart, histogramSet->mAxisDependy))));
        vertex.push_back(QPointF(translateGLPositionX(chart, xValuePosition(chart, entry->x+histogramSet->mDataSpan)), translateGLPositionY(chart, yValuePosition(chart, entry->yStart, histogramSet->mAxisDependy))));
        vertex.push_back(QPointF(translateGLPositionX(chart, xValuePosition(chart, entry->x+histogramSet->mDataSpan)), translateGLPositionY(chart, yValuePosition(chart, entry->yEnd, histogramSet->mAxisDependy))));
        vertex.push_back(QPointF(translateGLPositionX(chart, xValuePosition(chart, entry->x)), translateGLPositionY(chart, yValuePosition(chart, entry->yEnd, histogramSet->mAxisDependy))));

        if (histogramSet->mFillEnable) {
            glColor3f(histogramSet->mFillColor.red() / 255.0f, histogramSet->mFillColor.green() / 255.0f, histogramSet->mFillColor.blue() / 255.0f);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glBegin(GL_QUADS);

            glVertex3f(vertex[0].x(), vertex[0].y(), 0);
            glVertex3f(vertex[1].x(), vertex[1].y(),0);
            glVertex3f(vertex[2].x(), vertex[2].y(),0);
            glVertex3f(vertex[3].x(), vertex[3].y(),0);

            glEnd();
        }

        if (histogramSet->mBorderEnable) {
            glColor3f(histogramSet->mBorderColor.red() / 255.0f, histogramSet->mBorderColor.green() / 255.0f, histogramSet->mBorderColor.blue() / 255.0f);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            glBegin(GL_QUADS);

            glVertex3f(vertex[0].x(), vertex[0].y(), 0);
            glVertex3f(vertex[1].x(), vertex[1].y(), 0);
            glVertex3f(vertex[2].x(), vertex[2].y(), 0);
            glVertex3f(vertex[3].x(), vertex[3].y(), 0);

            glEnd();
        }
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
