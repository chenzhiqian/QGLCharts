#ifndef LINEDATASET_H
#define LINEDATASET_H

#include "qgldataset.h"


typedef enum
{
    LineContinuousMode,
    LineSplitMode
}LineMode;

class QGLLineChart;
class QGLLineSet : public QGLDataSet
{
private:
    friend class QGLLineChart;

    bool mFillEnable;                          // 预留: 死否绘制填充
    QVector<QColor> mFillColors;    // 预留: 折线图与x轴基线之间填充色
    LineMode mLineMode;                // 数据池中的相邻两个点，当x值相差大于mSplitInterval时，是否需要连线
    double mSplitInterval;                  // 分段间隔
    QColor mLineColor;                     // 折线颜色
    int mLineWidth;                           // 折线粗度

public:
    QGLLineSet(QColor lineColor, int lineWidth);
    virtual ~QGLLineSet();

    void setFillColor(QColor color);
    void setFillColors(QVector<QColor> colors);
    QVector<QColor> fillColors();
    void clearFillColor();

    LineMode lineMode() const;
    void setLineMode(const LineMode &lineMode);

    double splitInterval() const;
    void setSplitInterval(double splitInterval);

    QColor lineColor() const;
    void setLineColor(const QColor &lineColor);

    int lineWidth() const;
    void setLineWidth(int lineWidth);

    bool fillEnable() const;
    void setFillEnable(bool fillEnable);
};

#endif // LINEDATASET_H
