#ifndef QGLLINECHART_H
#define QGLLINECHART_H

#include "qglbasechart.h"
#include "qgllineset.h"

class QGLLineChart : public QGLBaseChart
{
private:
    void drawSet(QGLDataSet *set) override;

public:
    static void drawLineSet(QGLBaseChart *chart, QGLLineSet *lineSet);

public:
    explicit QGLLineChart(QWidget *parent = nullptr);
    virtual ~QGLLineChart();
};

#endif // QGLLINECHART_H
