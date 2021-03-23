#include "qglaxis.h"

bool QGLBaseAxis::drawBaseLine() const
{
    return mDrawBaseLine;
}

void QGLBaseAxis::setDrawBaseLine(bool drawBaseLine)
{
    mDrawBaseLine = drawBaseLine;
}

bool QGLBaseAxis::calibrationEnable() const
{
    return mCalibrationEnable;
}

void QGLBaseAxis::setCalibrationEnable(bool calibrationEnable)
{
    mCalibrationEnable = calibrationEnable;
}

unsigned int QGLBaseAxis::calibrationHeight() const
{
    return mCalibrationHeight;
}

void QGLBaseAxis::setCalibrationHeight(unsigned int calibrationHeight)
{
    mCalibrationHeight = calibrationHeight;
}

unsigned int QGLBaseAxis::majorCalibrationCount() const
{
    return mMajorCalibrationCount;
}

void QGLBaseAxis::setMajorCalibrationCount(unsigned int majorCalibrationCount)
{
    mMajorCalibrationCount = majorCalibrationCount;
}

unsigned int QGLBaseAxis::minorCalibrationCount() const
{
    return mMinorCalibrationCount;
}

void QGLBaseAxis::setMinorCalibrationCount(unsigned int minorCalibrationCount)
{
    mMinorCalibrationCount = minorCalibrationCount;
}

bool QGLBaseAxis::gridEnable() const
{
    return mGridEnable;
}

void QGLBaseAxis::setGridEnable(bool gridEnable)
{
    mGridEnable = gridEnable;
}

int QGLBaseAxis::gridCount() const
{
    return mGridCount;
}

void QGLBaseAxis::setGridCount(int gridCount)
{
    mGridCount = gridCount;
}

QColor QGLBaseAxis::gridColor() const
{
    return mGridColor;
}

void QGLBaseAxis::setGridColor(const QColor &gridColor)
{
    mGridColor = gridColor;
}

double QGLBaseAxis::gridWidth() const
{
    return mGridWidth;
}

void QGLBaseAxis::setGridWidth(double gridWidth)
{
    mGridWidth = gridWidth;
}

bool QGLBaseAxis::titleEnable() const
{
    return mTitleEnable;
}

void QGLBaseAxis::setTitleEnable(bool titleEnable)
{
    mTitleEnable = titleEnable;
}

int QGLBaseAxis::titleCount() const
{
    return mTitleCount;
}

void QGLBaseAxis::setTitleCount(int titleCount)
{
    mTitleCount = titleCount;
}

QFont QGLBaseAxis::titleFont() const
{
    return mTitleFont;
}

void QGLBaseAxis::setTitleFont(const QFont &titleFont)
{
    mTitleFont = titleFont;
}

QColor QGLBaseAxis::titleColor() const
{
    return mTitleColor;
}

void QGLBaseAxis::setTitleColor(const QColor &titleColor)
{
    mTitleColor = titleColor;
}

double QGLBaseAxis::showMin() const
{
    return mShowMin;
}

void QGLBaseAxis::setShowMin(double showMin)
{
    mShowMin = showMin;
}

double QGLBaseAxis::showMax() const
{
    return mShowMax;
}

void QGLBaseAxis::setShowMax(double showMax)
{
    mShowMax = showMax;
}

double QGLBaseAxis::titleAxisSpace() const
{
    return mTitleAxisSpace;
}

void QGLBaseAxis::setTitleAxisSpace(double titleAxisSpace)
{
    mTitleAxisSpace = titleAxisSpace;
}

void QGLBaseAxis::setTitleFormatter(const TitleFormatter &titleFormatter)
{
    mTitleFormatter = titleFormatter;
}

QVector<HighlightStru> QGLBaseAxis::highlightArea() const
{
    return mHighlightArea;
}

void QGLBaseAxis::appendHighlight(HighlightStru highlight)
{
    mHighlightArea.push_back(highlight);
}

void QGLBaseAxis::clearHighlight()
{
    mHighlightArea.clear();
}

QGLBaseAxis::QGLBaseAxis()
{
    mGridEnable = true;
    mGridCount = 5;
    mGridWidth = 1;
    mGridColor = Qt::black;
    mTitleEnable = true;
    mTitleColor = Qt::black;
    mTitleCount = 5;
    mTitleFont = QFont();

    axisMin = 0;
    axisMax = 0;
    mShowMin = 0;
    mShowMax = 0;

    mTitleAxisSpace = 10;

    mTitleFormatter = nullptr;

    mDrawBaseLine = true;
    mCalibrationEnable = false;
    mMajorCalibrationCount = 0;
    mMinorCalibrationCount = 0;
    mCalibrationHeight = 10;
}

void QGLBaseAxis::setRange(double min, double max)
{
    this->axisMin = min;
    this->mShowMin = min;
    this->axisMax = max;
    this->mShowMax = max;

    emit axisRangeChanged(this);
}

double QGLBaseAxis::min()
{
    return axisMin;
}

double QGLBaseAxis::max()
{
    return axisMax;
}

