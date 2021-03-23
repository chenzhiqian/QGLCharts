#include "qglscatterset.h"

QColor QGLScatterSet::pointColor() const
{
    return mPointColor;
}

void QGLScatterSet::setPointColor(const QColor &pointColor)
{
    mPointColor = pointColor;
}

float QGLScatterSet::pointSize() const
{
    return mPointSize;
}

void QGLScatterSet::setPointSize(float pointSize)
{
    mPointSize = pointSize;
}

QGLScatterSet::QGLScatterSet(QColor color, float size) : QGLDataSet(AxisDependyLeft)
{
    mSetType = ScatterType;
    mPointColor = color;
    mPointSize = size;
}
