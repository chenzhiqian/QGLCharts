#ifndef CHARTDATASET_H
#define CHARTDATASET_H

#include <QColor>

typedef enum {
    AxisDependyLeft,
    AxisDependyRight
} AxisDependy;

typedef enum {
    TypeUnknown,
    LineType,
    ScatterType,
    HistogramType,
} ChartSetType;

class ChartDataEntry
{
public:
    double x;
    double y;

    ChartDataEntry(double xValue, double yValue);
    virtual ~ChartDataEntry() = default;
};

class QGLBaseChart;
class QGLDataSet
{
protected:
    friend class QGLBaseChart;
    QVector<ChartDataEntry *> mEntryValues; // 数据池
    ChartSetType mSetType;      // 数据段类型
    AxisDependy mAxisDependy; // y值参考左侧y轴或右侧y轴

public:
    QGLDataSet(AxisDependy dependy);
    virtual ~QGLDataSet();

    AxisDependy axisDependy() const;
    void setAxisDependy(const AxisDependy &axisDependy);

    virtual void addEntry(ChartDataEntry *entry);
    void clear();
    QVector<ChartDataEntry *> entryValues();

    ChartSetType type();
};

#endif // CHARTDATASET_H
