#ifndef QGLSCATTERCHART_H
#define QGLSCATTERCHART_H

#include "qglbasechart.h"
#include "qglscatterset.h"

class QGLScatterChart : public QGLBaseChart
{
private:
    virtual void initializeGL() override;
    void drawSet(QGLDataSet *set) override;

public:
    static void drawScatterSet(QGLBaseChart *chart, QGLScatterSet *scatterSet);
public:
    explicit QGLScatterChart(QWidget *parent = nullptr);
    virtual ~QGLScatterChart();
};

#endif // QGLSCATTERCHART_H
