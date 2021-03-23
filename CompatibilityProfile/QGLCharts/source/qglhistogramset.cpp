#include "qglhistogramset.h"

HistogramDataEntry::HistogramDataEntry(double x, double yStart, double yEnd) : ChartDataEntry(x, yStart)
{
    this->yStart = yStart;
    this->yEnd = yEnd;
}

bool QGLHistogramSet::fillEnable() const
{
    return mFillEnable;
}

void QGLHistogramSet::setFillEnable(bool fillEnable)
{
    mFillEnable = fillEnable;
}

QColor QGLHistogramSet::fillColor() const
{
    return mFillColor;
}

void QGLHistogramSet::setFillColor(const QColor &fillColor)
{
    mFillColor = fillColor;
}

bool QGLHistogramSet::borderEnable() const
{
    return mBorderEnable;
}

void QGLHistogramSet::setBorderEnable(bool borderEnable)
{
    mBorderEnable = borderEnable;
}

QColor QGLHistogramSet::borderColor() const
{
    return mBorderColor;
}

void QGLHistogramSet::setBorderColor(const QColor &borderColor)
{
    mBorderColor = borderColor;
}

int QGLHistogramSet::dataSpan() const
{
    return mDataSpan;
}

void QGLHistogramSet::setDataSpan(int dataSpan)
{
    mDataSpan = dataSpan;
}

QGLHistogramSet::QGLHistogramSet(QColor fillColor, QColor borderColor, int span) : QGLDataSet(AxisDependyLeft)
{
    mSetType = HistogramType;
    mDataSpan = span;
    mFillColor = fillColor;
    mBorderColor = borderColor;
    mFillEnable = true;
    mBorderEnable = true;
}

void QGLHistogramSet::addEntry(ChartDataEntry *entry)
{
    if (entry == nullptr) {
        return;
    }

    HistogramDataEntry *histogramEntry = dynamic_cast<HistogramDataEntry *>(entry);
    if (histogramEntry == nullptr) {
        return;
    }

    QGLDataSet::addEntry(entry);
}
