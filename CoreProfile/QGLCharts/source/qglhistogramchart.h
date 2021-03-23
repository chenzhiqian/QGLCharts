#ifndef QGLHISTOGRAMCHART_H
#define QGLHISTOGRAMCHART_H

#include "qglbasechart.h"
#include "qglhistogramset.h"

class QGLHistogramChart  : public QGLBaseChart
{
private:
    void drawSet(QGLDataSet *set) override;    

public:
    static void drawHistogramSet(QGLBaseChart *chart, QGLHistogramSet *histogramSet);
public:
    explicit QGLHistogramChart(QWidget *parent = nullptr);
    virtual ~QGLHistogramChart();

};

#endif // QGLHISTOGRAMCHART_H
