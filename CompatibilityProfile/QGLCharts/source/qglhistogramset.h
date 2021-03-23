#ifndef QGLHISTOGRAMSET_H
#define QGLHISTOGRAMSET_H

#include "qgldataset.h"

class HistogramDataEntry : public ChartDataEntry
{
public:
    double yStart;      // 柱状图底部位置
    double yEnd;        // 柱状图顶部位置
public:
    HistogramDataEntry(double x, double yStart, double yEnd);
    virtual ~HistogramDataEntry() = default;
};

class QGLHistogramChart;
class QGLHistogramSet : public QGLDataSet
{
private:
    friend class QGLHistogramChart;
    bool mFillEnable;   // 是否填充柱状
    QColor mFillColor;  // 填充色
    bool mBorderEnable; // 是否绘制柱状边框
    QColor mBorderColor; // 边框色
    int mDataSpan;   // 柱状宽度(数据值跨度)

public:
    QGLHistogramSet(QColor fillColor, QColor borderColor, int span);
    virtual ~QGLHistogramSet() = default;
    void addEntry(ChartDataEntry *entry) override;

    bool fillEnable() const;
    void setFillEnable(bool fillEnable);

    QColor fillColor() const;
    void setFillColor(const QColor &fillColor);

    bool borderEnable() const;
    void setBorderEnable(bool borderEnable);

    QColor borderColor() const;
    void setBorderColor(const QColor &borderColor);

    int dataSpan() const;
    void setDataSpan(int dataSpan);
};

#endif // QGLHISTOGRAMSET_H
