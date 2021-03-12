#include "qgldataset.h"

ChartDataEntry::ChartDataEntry(double xValue, double yValue)
{
    x = xValue;
    y = yValue;
}

AxisDependy QGLDataSet::axisDependy() const
{
    return mAxisDependy;
}

void QGLDataSet::setAxisDependy(const AxisDependy &axisDependy)
{
    mAxisDependy = axisDependy;
}

QGLDataSet::QGLDataSet(AxisDependy dependy)
{
    mAxisDependy = dependy;
    mSetType = TypeUnknown;
}

QGLDataSet::~QGLDataSet()
{
    clear();
}

void QGLDataSet::addEntry(ChartDataEntry *entry)
{
    if (entry == nullptr) {
        return;
    }

    mEntryValues.push_back(entry);
}

void QGLDataSet::clear()
{
    int length = mEntryValues.size();
    for (int i=0;i<length;i++) {
        delete mEntryValues[i];
    }

    mEntryValues.clear();
}

QVector<ChartDataEntry *> QGLDataSet::entryValues()
{
    QVector<ChartDataEntry *> copyValues;
    copyValues.append(mEntryValues);

    return copyValues;
}

ChartSetType QGLDataSet::type()
{
    return mSetType;
}
