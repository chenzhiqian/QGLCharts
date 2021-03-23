#include "qglscatterchart.h"

QGLScatterChart::QGLScatterChart(QWidget *parent) : QGLBaseChart(parent)
{

}

QGLScatterChart::~QGLScatterChart()
{

}

void QGLScatterChart::initializeGL()
{
    QGLBaseChart::initializeGL();

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
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
    QVector<ChartDataEntry *> dataList = scatterSet->entryValues();
    int dataLength = dataList.size();
    if (dataLength == 0) {
        return;
    }

    double xAxisShowMin = chart->xAxis.showMin();
    double xAxisShowMax = chart->xAxis.showMax();

    double leftAxisShowMin = chart->leftAxis.showMin();
    double leftAxisShowMax = chart->leftAxis.showMax();

    double rightAxisShowMin = chart->rightAxis.showMin();
    double rightAxisShowMax = chart->rightAxis.showMax();

    glPointSize(scatterSet->mPointSize);
    glColor3f(scatterSet->mPointColor.red() / 255.0f, scatterSet->mPointColor.green() / 255.0f, scatterSet->mPointColor.blue() / 255.0f);
    glBegin(GL_POINTS);

    for (int i=0;i<dataLength;i++) {
        ChartDataEntry *entry = dataList[i];
        if (entry->x >= xAxisShowMin && entry->x <= xAxisShowMax) {
            if (scatterSet->mAxisDependy == AxisDependyLeft) {
                if (entry->y >= leftAxisShowMin && entry->y <= leftAxisShowMax) {
                    QPointF currentPoint = QPointF(xValuePosition(chart, entry->x), yValuePosition(chart, entry->y, scatterSet->mAxisDependy));
                    glVertex3f(translateGLPositionX(chart, currentPoint.x()), translateGLPositionY(chart, currentPoint.y()), 0);
                }
            }
            else if (scatterSet->mAxisDependy == AxisDependyRight) {
                if (entry->y >= rightAxisShowMin && entry->y <= rightAxisShowMax) {
                    QPointF currentPoint = QPointF(xValuePosition(chart, entry->x), yValuePosition(chart, entry->y, scatterSet->mAxisDependy));
                    glVertex3f(translateGLPositionX(chart, currentPoint.x()), translateGLPositionY(chart, currentPoint.y()), 0);
                }
            }
        }
    }

    glEnd();
}
