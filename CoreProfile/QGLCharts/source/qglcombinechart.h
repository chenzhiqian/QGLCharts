#ifndef QGLCOMBINECHART_H
#define QGLCOMBINECHART_H

#include "qglbasechart.h"

class QGLCombineChart : public QGLBaseChart
{
private:
    void drawSet(QGLDataSet *set) override;

public:
    explicit QGLCombineChart(QWidget *parent = nullptr);
    virtual ~QGLCombineChart() = default;
};

#endif // QGLCOMBINECHART_H
