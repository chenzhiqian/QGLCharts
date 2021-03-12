#ifndef BASEAXIS_H
#define BASEAXIS_H

#include <QColor>
#include <QFont>

typedef QString (*TitleFormatter)(double value);

#define MAJOR_MINOR_SCALE   (0.5)

typedef struct{
    int min;
    int max;
    QColor color;
} HighlightStru;

class QGLBaseChart;
class QGLBaseAxis : public QObject
{
    Q_OBJECT
protected:
    friend class QGLBaseChart;
    double axisMin;                // 坐标轴最小值
    double axisMax;               // 坐标轴最大值

    bool mDrawBaseLine;       // 是否绘制坐标轴基线

    bool mCalibrationEnable;    // 是否绘制刻度尺
    unsigned int mCalibrationHeight;             // 主刻度尺高度，次刻度50%
    unsigned int mMajorCalibrationCount;      // 坐标系主刻度个数
    unsigned int mMinorCalibrationCount;     // 每个主刻度之间绘制子刻度个数

    bool mGridEnable;           // 是否绘制网格
    int mGridCount;              // 网格个数
    QColor mGridColor;        // 网格线颜色
    double mGridWidth;        // 网格线粗度

    bool mTitleEnable;           // 是否绘制刻度值文本
    int mTitleCount;              // 刻度值文本个数
    QFont mTitleFont;           // 刻度值文本字体
    QColor mTitleColor;        // 刻度值文本颜色

    double mShowMin;        // 当前坐标轴显示的最小值
    double mShowMax;       // 当前坐标轴显示的最大值

    double mTitleAxisSpace;     // 刻度值文本与坐标轴基线间距
    TitleFormatter mTitleFormatter;   // 刻度值文本格式化外部实现
    QVector<HighlightStru> mHighlightArea;  // 高亮背景范围及颜色

public:
    QGLBaseAxis();

    virtual void setRange(double min, double max);
    double min();
    double max();

    bool drawBaseLine() const;
    void setDrawBaseLine(bool drawBaseLine);

    bool calibrationEnable() const;
    void setCalibrationEnable(bool calibrationEnable);

    unsigned int calibrationHeight() const;
    void setCalibrationHeight(unsigned int calibrationHeight);

    unsigned int majorCalibrationCount() const;
    void setMajorCalibrationCount(unsigned int majorCalibrationCount);

    unsigned int minorCalibrationCount() const;
    void setMinorCalibrationCount(unsigned int minorCalibrationCount);

    bool gridEnable() const;
    void setGridEnable(bool gridEnable);

    int gridCount() const;
    void setGridCount(int gridCount);

    QColor gridColor() const;
    void setGridColor(const QColor &gridColor);

    double gridWidth() const;
    void setGridWidth(double gridWidth);

    bool titleEnable() const;
    void setTitleEnable(bool titleEnable);

    int titleCount() const;
    void setTitleCount(int titleCount);

    QFont titleFont() const;
    void setTitleFont(const QFont &titleFont);

    QColor titleColor() const;
    void setTitleColor(const QColor &titleColor);

    double showMin() const;
    void setShowMin(double showMin);

    double showMax() const;
    void setShowMax(double showMax);

    double titleAxisSpace() const;
    void setTitleAxisSpace(double titleAxisSpace);

    void setTitleFormatter(const TitleFormatter &titleFormatter);

    QVector<HighlightStru> highlightArea() const;
    void appendHighlight(HighlightStru highlight);
    void clearHighlight();

signals:
    void axisRangeChanged(QGLBaseAxis *axis);
};

#endif // BASEAXIS_H
