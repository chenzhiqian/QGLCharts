#ifndef QGLBASECHART_H
#define QGLBASECHART_H

#include <QOpenGLWidget>
#include <QWidget>
#include "qglaxis.h"
#include "qgldataset.h"
#include <qevent.h>

class QGLBaseChart : public QOpenGLWidget
{
    Q_OBJECT
protected:
    QColor backgroundColor;     // 图标背景色

    bool enableXAxis;                 // 是否绘制x轴
    bool enableLeftAxis;             // 是否绘制左侧y轴
    bool enableRightAxis;           // 是否绘制右侧y轴

    double leftPadding;              // 左侧y轴基线与widget左侧边距
    double rightPadding;            // 右侧y轴基线与widget右侧边距
    double topPadding;              // x轴顶部基线与widget顶部边距
    double bottomPadding;        // x轴底部基线与widget底部边距
    double zoomScale;               // 当前缩放倍率
    double maxZoom;                // 最大缩放倍率
    double minZoom;                 // 最小缩放倍率
    QVector<QGLDataSet *> dataSets;   // 数据集合

    QSize contentSize;               // 图表原始内容区大小(主要影响因素: 分页、widget大小)
    QSize zoomContentSize;      // 图表缩放后内容区大小
    QPointF contentOffset;        // 图表当前显示左上角在缩放后内容区的位置

    bool mouseTrace;                 // 鼠标左击flag
    bool mouseMove;                 // 鼠标左击拖拽
    QPointF trackPoint;              // 鼠标点击图表位置

    bool enableSelected;             // 是否支持区域选择
    QColor selectedColor;           // 被选中区域遮罩颜色
    int selectedRowCount;          // 图表全局行分段个数
    int selectedColumnCount;     // 图标全局列分段个数

    AxisDependy selectedDependy;   // 选中区域y轴数据值依赖
    QRectF selectedRect;            // 选中数据值范围

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    static void drawArray(GLenum mode, double *vertices, int count);
    static void drawElement(GLenum mode, double *vertices, unsigned int vertexCount, unsigned int *elements, unsigned int elementCount);
    // 绘制图表高亮背景
    void drawHighlight();
    // 绘制坐标轴
    void drawAxis();
    // 绘制图表网格
    void drawGrid();
    // 绘制坐标系刻度
    void drawCalibration(QPainter &painter);
    // 绘制坐标系刻度值文本
    void drawTitle(QPainter &painter);
    // 绘制数据
    void drawData();
    // 绘制选中遮罩
    void drawSelected();
    // 绘制某条数据段
    virtual void drawSet(QGLDataSet *set);
    // 转换widget-x坐标系为opengl-x坐标系
    static double translateGLPositionX(QGLBaseChart *chart, double xPosition);
    // 转换widget-y坐标系为opengl-y坐标系
    static double translateGLPositionY(QGLBaseChart *chart, double yPosition);
    // 转换x数值对应widget坐标系x位置
    static double xValuePosition(QGLBaseChart *chart, double x);
    // 转换y数值对应widget坐标系y位置
    static double yValuePosition(QGLBaseChart *chart, double y, AxisDependy denpendy);

protected slots:
    void xAxisViewRatioChanged(double ratio);
    void axisRangeChanged(QGLBaseAxis *axis);
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
public:
    QGLXAxis xAxis;
    QGLYAxis leftAxis;
    QGLYAxis rightAxis;

public:
    explicit QGLBaseChart(QWidget *parent = nullptr);
    virtual ~QGLBaseChart();

    // 设置图表各个坐标轴基线与widget边距
    void setChartPadding(double left, double top, double right, double bottom);
    // 设置图表背景色
    void setBackgroundColor(QColor color);
    // 设置是否绘制x轴
    void setEnableXAxis(bool enable);
    // 设置是否绘制左侧y轴
    void setEnableLeftAxis(bool enable);
    // 设置是否绘制右侧y轴
    void setEnableRightAxis(bool enable);
    // 设置是否开启分区选择及相关配置
    void setEnableSelected(bool enable, int columnCount, int rowCount, QColor color = Qt::lightGray);

    // 获取当前图表所有数据段
    QVector<QGLDataSet *> dataList();
    // 添加数据段
    void appendData(QGLDataSet *data);
    // 移除数据段
    void removeData(QGLDataSet *data);
    // 清空数据段
    void clearData();
    // 设置缩放倍率
    bool setZoom(double minZoom, double maxZoom);

    // 滚动图表显示到指定位置
    void scrollXAxisShowMin(double showMin);
signals:
    // 当前图表x轴显示范围
    void willShowXAxis(double showMin, double showMax);
    // 图表被选中数据范围
    void selectedChanged(double xMin, double xMax, double leftMin, double leftMax, double rightMin, double rightMax);
};

#endif // QGLBASECHART_H
