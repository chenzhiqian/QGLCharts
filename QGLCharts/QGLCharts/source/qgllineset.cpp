#include "qgllineset.h"

LineMode QGLLineSet::lineMode() const
{
    return mLineMode;
}

void QGLLineSet::setLineMode(const LineMode &lineMode)
{
    mLineMode = lineMode;
}

double QGLLineSet::splitInterval() const
{
    return mSplitInterval;
}

void QGLLineSet::setSplitInterval(double splitInterval)
{
    mSplitInterval = splitInterval;
}

QColor QGLLineSet::lineColor() const
{
    return mLineColor;
}

void QGLLineSet::setLineColor(const QColor &lineColor)
{
    mLineColor = lineColor;
}

int QGLLineSet::lineWidth() const
{
    return mLineWidth;
}

void QGLLineSet::setLineWidth(int lineWidth)
{
    mLineWidth = lineWidth;
}

bool QGLLineSet::fillEnable() const
{
    return mFillEnable;
}

void QGLLineSet::setFillEnable(bool fillEnable)
{
    mFillEnable = fillEnable;
}

QGLLineSet::QGLLineSet(QColor lineColor, int lineWidth) : QGLDataSet(AxisDependyLeft)
{
    mLineColor = lineColor;
    mLineWidth = lineWidth;
    mLineMode = LineContinuousMode;
    mSplitInterval = 3000;
    mSetType = LineType;

    mFillEnable = false;
}

QGLLineSet::~QGLLineSet()
{

}

void QGLLineSet::setFillColor(QColor color)
{
    mFillColors.clear();
    mFillColors.push_back(color);
}

void QGLLineSet::setFillColors(QVector<QColor> colors)
{
    mFillColors.clear();
    mFillColors.append(colors);
}

QVector<QColor>QGLLineSet::fillColors()
{
    QVector<QColor> copyValues;
    copyValues.append(mFillColors);

    return copyValues;
}

void QGLLineSet::clearFillColor()
{
    mFillColors.clear();
}
