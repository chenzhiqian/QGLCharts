#include "qglaxis.h"

QGLXAxis::QGLXAxis()
{
    mTitlePosition = XAxisTitleBottom;
    mViewRatio = 1;
}

void QGLXAxis::setRange(double min, double max)
{
    this->axisMin = min;
    this->mShowMin = min;
    this->axisMax = max;
    this->mShowMax = min + (max-min) * mViewRatio;

    emit axisRangeChanged(this);
}

void QGLXAxis::setTitlePosition(XAxisTitlePosition position)
{
    this->mTitlePosition = position;
}

XAxisTitlePosition QGLXAxis::titlePosition()
{
    return mTitlePosition;
}

double QGLXAxis::viewRatio()
{
    return mViewRatio;
}

void QGLXAxis::setViewRatio(double ratio)
{
    if (ratio > 1) {
        ratio = 1;
    }
    this->mViewRatio = ratio;
    this->mShowMin = this->axisMin;
    this->mShowMax = this->axisMin + (this->axisMax - this->axisMin) * ratio;

    emit viewRatioChanged(ratio);
}

QGLYAxis::QGLYAxis()
{
    mTitlePosition = YAxisBaselineLeft;
}

void QGLYAxis::setTitlePosition(YAxisTitlePosition position)
{
    this->mTitlePosition = position;
}

YAxisTitlePosition QGLYAxis::titlePosition()
{
    return mTitlePosition;
}
