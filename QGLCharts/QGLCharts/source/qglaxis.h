#ifndef CHARTAXIS_H
#define CHARTAXIS_H

#include "qglbaseaxis.h"

typedef enum {
    XAxisTitleTop,
    XAxisTitleTopInner,
    XAxisTitleBottom,
    XAxisTitleBottomInner
} XAxisTitlePosition;

typedef enum {
    YAxisBaselineLeft,
    YAxisBaselineRight
} YAxisTitlePosition;

class QGLBaseChart;
class QGLXAxis : public QGLBaseAxis
{
    Q_OBJECT
private:
    friend class QGLBaseChart;
    double mViewRatio;      // 分页比例
    XAxisTitlePosition mTitlePosition;  // 坐标轴刻度值显示位置

public:
    QGLXAxis();
    /*
     * 设置X坐标轴范围
     */
    void setRange(double min, double max) override;

    /*
     * 设置坐标轴刻度值显示位置
     */
    void setTitlePosition(XAxisTitlePosition position);

    /*
     * 坐标轴刻度值显示位置
     */
    XAxisTitlePosition titlePosition();

    /*
     * 设置分页比例, 每页占总长度比例[0, 1]
     */
    void setViewRatio(double ratio);

    /*
     * 获取分页比例
     */
    double viewRatio();
signals:
    void viewRatioChanged(double ratio);
};

class QGLYAxis : public QGLBaseAxis
{
    Q_OBJECT
private:
    friend class QGLBaseChart;
    YAxisTitlePosition mTitlePosition;  // 坐标轴刻度值显示位置
public:
    QGLYAxis();
    /*
     * 设置y轴刻度值显示位置
     */
    void setTitlePosition(YAxisTitlePosition position);

    /*
     * y轴刻度值显示位置
     */
    YAxisTitlePosition titlePosition();
};

#endif // CHARTAXIS_H
