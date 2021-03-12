#include "qglcombinechart.h"
#include "qgllinechart.h"
#include "qglscatterchart.h"
#include "qglhistogramchart.h"
QGLCombineChart::QGLCombineChart(QWidget *parent) : QGLBaseChart(parent)
{

}

void QGLCombineChart::drawSet(QGLDataSet *set)
{
    // 根据不同的数据段类型，调用不同的绘图接口
    switch(set->type()) {
    case LineType:
        QGLLineChart::drawLineSet(this, (QGLLineSet *)set);
        break;
    case ScatterType:
        QGLScatterChart::drawScatterSet(this, (QGLScatterSet *)set);
        break;
    case HistogramType:
        QGLHistogramChart::drawHistogramSet(this, (QGLHistogramSet *)set);
        break;
    default:
        break;
    }
}
