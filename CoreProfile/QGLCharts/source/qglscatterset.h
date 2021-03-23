#ifndef QGLSCATTERSET_H
#define QGLSCATTERSET_H

#include "qgldataset.h"

class QGLScatterChart;
class QGLScatterSet : public QGLDataSet
{
private:
    friend class QGLScatterChart;
    QColor mPointColor;     // 散点颜色
    float mPointSize;           // 散点直径

public:
    QGLScatterSet(QColor color, float size);

    QColor pointColor() const;
    void setPointColor(const QColor &pointColor);

    float pointSize() const;
    void setPointSize(float pointSize);
};

#endif // QGLSCATTERSET_H
