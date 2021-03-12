#include "qglbasechart.h"
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include <QPen>
#include <QDebug>
#include <QApplication>
#include <QDateTime>
#include <math.h>
#include <QStyleOption>

#define LEFT_PADDING_DEFAULT    (30)
#define RIGHT_PADDING_DEFAULT    (5)
#define TOP_PADDING_DEFAULT    (10)
#define BOTTOM_PADDING_DEFAULT    (30)

QGLBaseChart::QGLBaseChart(QWidget *parent) : QOpenGLWidget(parent)
{    
    // 设置多采样参数，实现opengl抗锯齿
    QSurfaceFormat surfaceFormat;
    surfaceFormat.setSamples(8);
    setFormat(surfaceFormat);

    enableXAxis = true;
    enableLeftAxis = true;
    rightAxis.mTitlePosition = YAxisBaselineRight;
    enableRightAxis = false;

    leftPadding = LEFT_PADDING_DEFAULT;
    rightPadding = RIGHT_PADDING_DEFAULT;
    topPadding = TOP_PADDING_DEFAULT;
    bottomPadding = BOTTOM_PADDING_DEFAULT;

    dataSets.clear();

    zoomScale = 1;
    maxZoom = 2;
    minZoom = 1;

    contentSize = QSize(width() - leftPadding - rightPadding, height() - topPadding - bottomPadding);
    zoomContentSize = QSize(contentSize.width() * zoomScale, contentSize.height());
    contentOffset = QPointF(0, 0);
    connect(&xAxis, SIGNAL(viewRatioChanged(double)), this, SLOT(xAxisViewRatioChanged(double)));
    connect(&xAxis, SIGNAL(axisRangeChanged(QGLBaseAxis *)), this, SLOT(axisRangeChanged(QGLBaseAxis *)));
    mouseTrace = false;
    mouseMove = false;
    backgroundColor = Qt::white;

    enableSelected = false;
    selectedColor = Qt::lightGray;
    selectedRowCount = 1;
    selectedColumnCount = 10;
    selectedRect = QRectF(0,0,0,0);
}

QGLBaseChart::~QGLBaseChart()
{
    clearData();
}

void QGLBaseChart::setChartPadding(double left, double top, double right, double bottom)
{
    leftPadding = left;
    topPadding = top;
    rightPadding = right;
    bottomPadding = bottom;

    contentSize = QSize((width() - leftPadding - rightPadding) / xAxis.viewRatio(), height() - topPadding - bottomPadding);
    zoomContentSize = QSize(contentSize.width() * zoomScale, contentSize.height());
}

void QGLBaseChart::setBackgroundColor(QColor color)
{
    this->backgroundColor = color;
}

void QGLBaseChart::setEnableXAxis(bool enable)
{
    this->enableXAxis = enable;
}

void QGLBaseChart::setEnableLeftAxis(bool enable)
{
    this->enableLeftAxis = enable;
}

void QGLBaseChart::setEnableRightAxis(bool enable)
{
    this->enableRightAxis = enable;
}

void QGLBaseChart::setEnableSelected(bool enable, int columnCount, int rowCount, QColor color)
{
    this->enableSelected = enable;
    this->selectedColumnCount = columnCount;
    this->selectedRowCount = rowCount;
    this->selectedColor = color;
    this->selectedRect = QRectF(0, 0, 0, 0);
}

void QGLBaseChart::xAxisViewRatioChanged(double ratio)
{
    // 坐标轴分页变更，调整内容区大小
    contentSize = QSize((width() - leftPadding - rightPadding) / ratio, height() - topPadding - bottomPadding);
    zoomContentSize = QSize(contentSize.width() * zoomScale, contentSize.height());
}

void QGLBaseChart::axisRangeChanged(QGLBaseAxis *axis)
{
    // 坐标轴范围变更，重置选择
    Q_UNUSED(axis);
    selectedRect = QRectF(0, 0, 0, 0);
}

void QGLBaseChart::appendData(QGLDataSet *data)
{
    dataSets.push_back(data);
}

void QGLBaseChart::removeData(QGLDataSet *data)
{
    for (QVector<QGLDataSet *>::iterator it=dataSets.begin();it!=dataSets.end();it++) {
        if (*it == data) {
            delete data;
            dataSets.erase(it);
            break;
        }
    }
}

void QGLBaseChart::clearData()
{
    int length = dataSets.size();
    for (int i=0;i<length;i++) {
        delete dataSets[i];
    }

    dataSets.clear();
}

QVector<QGLDataSet *> QGLBaseChart::dataList()
{
    QVector<QGLDataSet *>vt;
    vt.append(dataSets);

    return vt;
}

bool QGLBaseChart::setZoom(double min, double max)
{
    if (min > max) {
        return  false;
    }

    minZoom = min;
    maxZoom = max;
    zoomScale = 1;  // 重新设置缩放倍率时，强制重置显示倍率，可优化

    return true;
}

void QGLBaseChart::scrollXAxisShowMin(double showMin)
{
    if (showMin < xAxis.min()) {
        showMin = xAxis.min();
    }
    double duration = this->xAxis.max() - this->xAxis.min();
    double pageRation = (width() - leftPadding - rightPadding) / zoomContentSize.width();

    double tempShowMax = xAxis.max() - duration*pageRation;
    if (showMin > tempShowMax){
        showMin = tempShowMax;
    }

    double leftRatio = duration == 0 ? 0 : (showMin - xAxis.min())/ duration;

    this->xAxis.mShowMin = leftRatio * duration + this->xAxis.min();
    this->xAxis.mShowMax = this->xAxis.mShowMin + pageRation * duration;

    contentOffset = QPointF(zoomContentSize.width()*leftRatio , 0);

    update();
}

void QGLBaseChart::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    // 触发opengl绘图数据
    QOpenGLWidget::paintEvent(event);

    // 清理内容区外的数据绘制
    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.fillRect(QRectF(0, 0, width(), topPadding), backgroundColor);
    painter.fillRect(QRectF(0, 0, leftPadding-leftAxis.mGridWidth, height()), backgroundColor);
    painter.fillRect(QRectF(0, height()-bottomPadding + leftAxis.mGridWidth, width(), bottomPadding), backgroundColor);
    painter.fillRect(QRectF(width()-rightPadding + xAxis.mGridWidth, 0, rightPadding, height()), backgroundColor);

    drawTitle(painter);

    drawCalibration(painter);
}

void QGLBaseChart::wheelEvent(QWheelEvent *event)
{
    if (event->position().x() < leftPadding || event->position().x() > width() - rightPadding
            || event->position().y() <topPadding || event->position().y() > height() - bottomPadding) {
        return;
    }
    // 获取鼠标所在位置
    QPointF mousePoint  = QPointF(event->position().x() - leftPadding, event->position().y() - topPadding);

    /************** 根据滚动滚动浮动调整，缩放倍率 start *************/
    double zoom = event->delta() / 120 * 0.5 ;

    double preScale = zoomScale;

    if (zoomScale + zoom > maxZoom) {
        zoomScale = maxZoom;
    }
    else if (zoomScale + zoom < minZoom) {
        zoomScale = minZoom;
    }
    else {
        zoomScale += zoom;
    }
    /************** 根据滚动滚动浮动调整，缩放倍率 end *************/

    zoomContentSize = QSize(contentSize.width() * zoomScale, contentSize.height());

    /************** 根据鼠标所在位置进行缩放 start *************/
    double pointXInScroll = contentOffset.x() + mousePoint.x();
    double willStartShowScrollX = (zoomScale / preScale) * pointXInScroll - mousePoint.x();
    if (willStartShowScrollX < 0) {
        willStartShowScrollX = 0;
    }
    else if (willStartShowScrollX > zoomContentSize.width() - (width() - leftPadding - rightPadding)) {
        willStartShowScrollX = zoomContentSize.width() - (width() - leftPadding - rightPadding);
    }

    double leftRatio = willStartShowScrollX / this->zoomContentSize.width();
    double duration = this->xAxis.max() - this->xAxis.min();
    double pageRation = (width() - leftPadding - rightPadding) / zoomContentSize.width();

    this->xAxis.mShowMin = leftRatio * duration + this->xAxis.min();
    this->xAxis.mShowMax = this->xAxis.mShowMin + pageRation * duration;
    contentOffset = QPointF(willStartShowScrollX , 0);

    /************** 根据鼠标所在位置进行缩放 end *************/

    update();
}

void QGLBaseChart::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    double offsetRatio = contentOffset.x() / zoomContentSize.width();

    contentSize = QSize((width() - leftPadding - rightPadding) / xAxis.viewRatio(), height() - topPadding - bottomPadding);
    zoomContentSize = QSize(contentSize.width() * zoomScale, contentSize.height());

    contentOffset = QPointF(zoomContentSize.width() * offsetRatio, 0);

    QOpenGLWidget::resizeEvent(event);
}

void QGLBaseChart::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        if (event->pos().x() < leftPadding || event->pos().x() > width() - rightPadding
                || event->pos().y() <topPadding || event->pos().y() > height() - bottomPadding) {
            return;
        }

        mouseTrace = true;
        trackPoint = QPointF(event->pos().x() - leftPadding, event->pos().y() - topPadding);
    }
}

void QGLBaseChart::mouseMoveEvent(QMouseEvent *event)
{
    if (mouseTrace) {
        if (!mouseMove) {
            QCursor cursor;
            cursor.setShape(Qt::ClosedHandCursor);
            QApplication::setOverrideCursor(cursor);

            mouseMove = true;
        }

        QPointF currentPoint  = QPointF(event->pos().x() - leftPadding, event->pos().y() - topPadding);

        double willStartShowScrollX = contentOffset.x() - (currentPoint.x() - trackPoint.x());
        if (willStartShowScrollX < 0) {
            willStartShowScrollX = 0;
        }
        else if (willStartShowScrollX > zoomContentSize.width() - (width() - leftPadding - rightPadding)) {
            willStartShowScrollX = zoomContentSize.width() - (width() - leftPadding - rightPadding);
        }

        double leftRatio = willStartShowScrollX / this->zoomContentSize.width();
        double duration = this->xAxis.max() - this->xAxis.min();
        double pageRation = (width() - leftPadding - rightPadding) / zoomContentSize.width();

        this->xAxis.mShowMin = leftRatio * duration + this->xAxis.min();
        this->xAxis.mShowMax = this->xAxis.mShowMin + pageRation * duration;

        contentOffset = QPointF(willStartShowScrollX , 0);

        trackPoint = currentPoint;

        update();
    }
}

void QGLBaseChart::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mouseTrace = false;
        trackPoint = QPointF(event->pos().x() - leftPadding, event->pos().y() - topPadding);
        if (mouseMove) {
            QCursor cursor;
            cursor.setShape(Qt::ArrowCursor);
            QApplication::setOverrideCursor(cursor);
            mouseMove = false;
        }
        else {   // 鼠标点击松开时，如果之前没有发生移动, 则触发区域选择
            if (!enableSelected || selectedRowCount == 0 || selectedColumnCount == 0) {
                return;
            }

            double xPecent = trackPoint.x() / (width() - leftPadding - rightPadding);
            double yPecent = 1 - trackPoint.y() / (height() - topPadding - bottomPadding);

            double xValue = xAxis.mShowMin + (xAxis.mShowMax - xAxis.mShowMin) * xPecent;
            double yLeftValue = leftAxis.mShowMin + (leftAxis.mShowMax - leftAxis.mShowMin) * yPecent;
            double yRightValue = rightAxis.mShowMin + (rightAxis.mShowMax - rightAxis.mShowMin) * yPecent;

            double xSelectedInterval = fabs((xAxis.axisMax - xAxis.axisMin) / selectedColumnCount);
            double yLeftSelectedInterval = fabs((leftAxis.axisMax - leftAxis.axisMin) / selectedRowCount);
            double yRightSelectedInterval = fabs((rightAxis.axisMax - rightAxis.axisMin) / selectedRowCount);

            unsigned int xSelectedIndex = xSelectedInterval > 0 ? fabs((xValue - xAxis.axisMin) / xSelectedInterval) : 0;
            unsigned int yLeftSelectedIndex = yLeftSelectedInterval > 0 ? fabs((yLeftValue - leftAxis.axisMin) / yLeftSelectedInterval) : 0;
            unsigned int yRightSelectedIndex = yRightSelectedInterval > 0 ? fabs((yRightValue - rightAxis.axisMin) / yRightSelectedInterval) : 0;

            /*
             * 为了避免y轴最大值与最小值相同时，导致图表行分段选择失效。
             */
            if (leftAxis.axisMax == leftAxis.axisMin) {   // 左侧y轴范围未定义时，使用右侧y轴范围，界定选择区域范围
                selectedDependy = AxisDependyRight;
                selectedRect = QRectF(xAxis.axisMin+xSelectedIndex*xSelectedInterval, rightAxis.axisMin + yRightSelectedIndex*yRightSelectedInterval, xSelectedInterval, yRightSelectedInterval);
            }
            else {  // 右侧y轴范围未定义时，使用左侧y轴范围, 界定选择区域范围
                selectedDependy = AxisDependyLeft;
                selectedRect = QRectF(xAxis.axisMin+xSelectedIndex*xSelectedInterval, leftAxis.axisMin + yLeftSelectedIndex*yLeftSelectedInterval, xSelectedInterval, yLeftSelectedInterval);
            }
            emit selectedChanged(xAxis.axisMin+xSelectedIndex*xSelectedInterval, xAxis.axisMin + (xSelectedIndex+1)*xSelectedInterval,
                                 leftAxis.axisMin + yLeftSelectedIndex*yLeftSelectedInterval, leftAxis.axisMin + (yLeftSelectedIndex+1)*yLeftSelectedInterval,
                                 rightAxis.axisMin + yRightSelectedIndex*yRightSelectedInterval, rightAxis.axisMin + (yRightSelectedIndex+1)*yRightSelectedInterval);

            update();
        }
    }
}

void QGLBaseChart::initializeGL()
{
    // 设置opengl清屏颜色
    glClearColor(backgroundColor.red() / 255.0f, backgroundColor.green() / 255.0f, backgroundColor.blue() / 255.0f, backgroundColor.alpha() / 255.0f);
}

void QGLBaseChart::resizeGL(int w, int h)
{
    //opengl窗口矩阵转换
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
}

void QGLBaseChart::paintGL()
{
    // 开启opengl多采样抗锯齿
    glEnable(GL_MULTISAMPLE);

    //绘图区清屏
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawHighlight();
    drawGrid();
    drawData();
    drawAxis();
    drawSelected();

    glFlush();
}

void QGLBaseChart::drawHighlight()
{
    // 高亮背景区可能重叠，开启透明度混合模式，默认透明度0.8f
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    if (enableXAxis) {
        for (int i=0;i<xAxis.mHighlightArea.size();i++) {
            HighlightStru highlight = xAxis.mHighlightArea[i];

             glColor4f(highlight.color.red() / 255.0f, highlight.color.green() / 255.0f, highlight.color.blue() / 255.0f, 0.8f);
             glBegin(GL_QUADS);

             glVertex3f(translateGLPositionX(this, xValuePosition(this, highlight.min)), translateGLPositionY(this, height() - bottomPadding), 0);
             glVertex3f(translateGLPositionX(this, xValuePosition(this, highlight.max)), translateGLPositionY(this, height() - bottomPadding), 0);
             glVertex3f(translateGLPositionX(this, xValuePosition(this, highlight.max)), translateGLPositionY(this, topPadding), 0);
             glVertex3f(translateGLPositionX(this, xValuePosition(this, highlight.min)), translateGLPositionY(this, topPadding), 0);

             glEnd();
        }
    }

    if (enableLeftAxis) {
        for (int i=0;i<leftAxis.mHighlightArea.size();i++) {
            HighlightStru highlight = leftAxis.mHighlightArea[i];

             glColor4f(highlight.color.red() / 255.0f, highlight.color.green() / 255.0f, highlight.color.blue() / 255.0f, 0.8f);
             glBegin(GL_QUADS);

             glVertex3f(translateGLPositionX(this, leftPadding), translateGLPositionY(this, yValuePosition(this, highlight.min, AxisDependyLeft)), 0);
             glVertex3f(translateGLPositionX(this, width() - rightPadding), translateGLPositionY(this, yValuePosition(this, highlight.min, AxisDependyLeft)), 0);
             glVertex3f(translateGLPositionX(this, width() - rightPadding), translateGLPositionY(this, yValuePosition(this, highlight.max, AxisDependyLeft)), 0);
             glVertex3f(translateGLPositionX(this, leftPadding), translateGLPositionY(this, yValuePosition(this, highlight.max, AxisDependyLeft)), 0);

             glEnd();
        }
    }

    if (enableRightAxis) {
        for (int i=0;i<rightAxis.mHighlightArea.size();i++) {
            HighlightStru highlight = rightAxis.mHighlightArea[i];

             glColor4f(highlight.color.red() / 255.0f, highlight.color.green() / 255.0f, highlight.color.blue() / 255.0f, 0.8f);
             glBegin(GL_QUADS);

             glVertex3f(translateGLPositionX(this, leftPadding), translateGLPositionY(this, yValuePosition(this, highlight.min, AxisDependyRight)), 0);
             glVertex3f(translateGLPositionX(this, width() - rightPadding), translateGLPositionY(this, yValuePosition(this, highlight.min, AxisDependyRight)), 0);
             glVertex3f(translateGLPositionX(this, width() - rightPadding), translateGLPositionY(this, yValuePosition(this, highlight.max, AxisDependyRight)), 0);
             glVertex3f(translateGLPositionX(this, leftPadding), translateGLPositionY(this, yValuePosition(this, highlight.max, AxisDependyRight)), 0);

             glEnd();
        }
    }

    // 关闭混合模式
    glDisable(GL_BLEND);
}

void QGLBaseChart::drawAxis()
{
    glBegin(GL_LINES);

    if (enableXAxis && xAxis.mDrawBaseLine) {
        glLineWidth(xAxis.mGridWidth);
        glColor3f(xAxis.mGridColor.red() / 255.0f, xAxis.mGridColor.green() / 255.0f, xAxis.mGridColor.blue() / 255.0f);

        glVertex3f(translateGLPositionX(this, leftPadding), translateGLPositionY(this, height() - bottomPadding), 0);
        glVertex3f(translateGLPositionX(this, width() - rightPadding), translateGLPositionY(this, height() - bottomPadding), 0);
    }

    if (enableLeftAxis && leftAxis.mDrawBaseLine) {
        glLineWidth(leftAxis.mGridWidth);
        glColor3f(leftAxis.mGridColor.red() / 255.0f, leftAxis.mGridColor.green() / 255.0f, leftAxis.mGridColor.blue() / 255.0f);

        glVertex3f(translateGLPositionX(this, leftPadding), translateGLPositionY(this, topPadding), 0);
        glVertex3f(translateGLPositionX(this, leftPadding), translateGLPositionY(this, height() - bottomPadding), 0);
    }

    if (enableRightAxis && rightAxis.mDrawBaseLine) {
        glLineWidth(rightAxis.mGridWidth);
        glColor3f(rightAxis.mGridColor.red() / 255.0f, rightAxis.mGridColor.green() / 255.0f, rightAxis.mGridColor.blue() / 255.0f);

        glVertex3f(translateGLPositionX(this, width() - rightPadding), translateGLPositionY(this, topPadding), 0);
        glVertex3f(translateGLPositionX(this, width() - rightPadding), translateGLPositionY(this, height() - bottomPadding), 0);
    }

    glEnd();
}

void QGLBaseChart::drawGrid()
{
    glBegin(GL_LINES);

    if (enableXAxis) {
        glLineWidth(xAxis.mGridWidth);
        glColor3f(xAxis.mGridColor.red() / 255.0f, xAxis.mGridColor.green() / 255.0f, xAxis.mGridColor.blue() / 255.0f);

        if (xAxis.mGridEnable) {
            double interval = (width() - leftPadding - rightPadding) / xAxis.mGridCount;

            for (int i=1;i<=xAxis.mGridCount;i++) {
                glVertex3f(translateGLPositionX(this, leftPadding + i*interval), translateGLPositionY(this, topPadding), 0);
                glVertex3f(translateGLPositionX(this, leftPadding + i*interval), translateGLPositionY(this, height() - bottomPadding), 0);
            }
        }
    }

    if (enableLeftAxis) {

        glLineWidth(leftAxis.mGridWidth);
        glColor3f(leftAxis.mGridColor.red() / 255.0f, leftAxis.mGridColor.green() / 255.0f, leftAxis.mGridColor.blue() / 255.0f);

        if (leftAxis.mGridEnable) {
            double interval = (height() - topPadding - bottomPadding) / leftAxis.mGridCount;

            for (int i=0;i<leftAxis.mGridCount;i++) {
                glVertex3f(translateGLPositionX(this, leftPadding), translateGLPositionY(this, topPadding+i*interval), 0);
                glVertex3f(translateGLPositionX(this, width() - rightPadding), translateGLPositionY(this, topPadding+i*interval), 0);
            }
        }
    }

    if (enableRightAxis) {
        glLineWidth(rightAxis.mGridWidth);
        glColor3f(rightAxis.mGridColor.red() / 255.0f, rightAxis.mGridColor.green() / 255.0f, rightAxis.mGridColor.blue() / 255.0f);

        if (rightAxis.mGridEnable) {
            double interval = (height() - topPadding - bottomPadding) / rightAxis.mGridCount;

            for (int i=0;i<rightAxis.mGridCount;i++) {
                glVertex3f(translateGLPositionX(this, leftPadding), translateGLPositionY(this, topPadding+i*interval), 0);
                glVertex3f(translateGLPositionX(this, width() - rightPadding), translateGLPositionY(this, topPadding+i*interval), 0);
            }
        }
    }

    glEnd();
}

void QGLBaseChart::drawCalibration(QPainter &painter)
{
    if (enableXAxis && xAxis.mCalibrationEnable && xAxis.mMajorCalibrationCount > 0) {
        QPen pen;
        pen.setColor(xAxis.mGridColor);
        painter.setPen(pen);

        double majorCalibrationInterval = (width() - leftPadding - rightPadding) / xAxis.mMajorCalibrationCount;
        double minorCalibrationInterval = majorCalibrationInterval / xAxis.mMinorCalibrationCount;

        double majorMinY = 0;
        double minorMinY = 0;
        switch (xAxis.mTitlePosition) {
        case XAxisTitleTop:
            majorMinY = topPadding - xAxis.mCalibrationHeight;
            minorMinY = topPadding - xAxis.mCalibrationHeight * MAJOR_MINOR_SCALE;
            break;
        case XAxisTitleTopInner:
            majorMinY = topPadding;
            minorMinY = topPadding;
            break;
        case XAxisTitleBottom:
            majorMinY = height() - bottomPadding;
            minorMinY = height() - bottomPadding;
            break;
        case XAxisTitleBottomInner:
            majorMinY = height() - bottomPadding - xAxis.mCalibrationHeight;
            minorMinY = height() - bottomPadding - xAxis.mCalibrationHeight * MAJOR_MINOR_SCALE;
            break;
        }

        for (unsigned int i=0;i<=xAxis.mMajorCalibrationCount;i++) {
            double majorPosition = leftPadding + i*majorCalibrationInterval;
            painter.drawLine(QPointF(majorPosition, majorMinY), QPointF(majorPosition, majorMinY+xAxis.mCalibrationHeight));

            if (i == xAxis.mMajorCalibrationCount) {
                break;
            }

            for (unsigned int j=1;j<xAxis.mMinorCalibrationCount;j++) {
                double minorPosition = majorPosition + j*minorCalibrationInterval;
                painter.drawLine(QPointF(minorPosition, minorMinY), QPointF(minorPosition, minorMinY+xAxis.mCalibrationHeight*MAJOR_MINOR_SCALE));
            }
        }
    }

    if (enableLeftAxis && leftAxis.mCalibrationEnable && leftAxis.mMajorCalibrationCount > 0) {
        QPen pen;
        pen.setColor(leftAxis.mGridColor);
        painter.setPen(pen);

        double majorCalibrationInterval = (height() - topPadding - bottomPadding) / leftAxis.mMajorCalibrationCount;
        double minorCalibrationInterval = majorCalibrationInterval / leftAxis.mMinorCalibrationCount;

        QFontMetrics fm = painter.fontMetrics();
        double majorMinX = 0;
        double minorMinX = 0;
        switch (leftAxis.mTitlePosition) {
        case YAxisBaselineLeft:
            majorMinX = leftPadding - leftAxis.mCalibrationHeight;
            minorMinX = leftPadding - leftAxis.mCalibrationHeight * MAJOR_MINOR_SCALE;
            break;
        case YAxisBaselineRight:
            majorMinX = leftPadding;
            minorMinX = leftPadding;
            break;
        }

        for (unsigned int i=0;i<=leftAxis.mMajorCalibrationCount;i++) {
            double majorPosition = topPadding + i*majorCalibrationInterval;
            painter.drawLine(QPointF(majorMinX, majorPosition), QPointF(majorMinX+leftAxis.mCalibrationHeight, majorPosition));

            if (i == leftAxis.mMajorCalibrationCount) {
                break;
            }

            for (unsigned int j=1;j<leftAxis.mMinorCalibrationCount;j++) {
                double minorPosition = majorPosition + j*minorCalibrationInterval;
                painter.drawLine(QPointF(minorMinX, minorPosition), QPointF(minorMinX+leftAxis.mCalibrationHeight * MAJOR_MINOR_SCALE, minorPosition));
            }
        }
    }

    if (enableRightAxis && rightAxis.mCalibrationEnable && rightAxis.mMajorCalibrationCount > 0) {
        QPen pen;
        pen.setColor(rightAxis.mGridColor);
        painter.setPen(pen);

        double majorCalibrationInterval = (height() - topPadding - bottomPadding) / rightAxis.mMajorCalibrationCount;
        double minorCalibrationInterval = majorCalibrationInterval / rightAxis.mMinorCalibrationCount;

        QFontMetrics fm = painter.fontMetrics();
        double majorMinX = 0;
        double minorMinX = 0;
        switch (rightAxis.mTitlePosition) {
        case YAxisBaselineLeft:
            majorMinX = width() - rightPadding - rightAxis.mCalibrationHeight;
            minorMinX = width() - rightPadding - rightAxis.mCalibrationHeight * MAJOR_MINOR_SCALE;
            break;
        case YAxisBaselineRight:
            majorMinX = width() - rightPadding;
            minorMinX = width() - rightPadding;
            break;
        }

        for (unsigned int i=0;i<=rightAxis.mMajorCalibrationCount;i++) {
            double majorPosition = topPadding + i*majorCalibrationInterval;
            painter.drawLine(QPointF(majorMinX, majorPosition), QPointF(majorMinX+rightAxis.mCalibrationHeight, majorPosition));

            if (i == rightAxis.mMajorCalibrationCount) {
                break;
            }

            for (unsigned int j=1;j<rightAxis.mMinorCalibrationCount;j++) {
                double minorPosition = majorPosition + j*minorCalibrationInterval;
                painter.drawLine(QPointF(minorMinX, minorPosition), QPointF(minorMinX+rightAxis.mCalibrationHeight * MAJOR_MINOR_SCALE, minorPosition));
            }
        }
    }
}

void QGLBaseChart::drawTitle(QPainter &painter)
{
    if (enableXAxis && xAxis.mTitleEnable) {
        QPen pen;
        pen.setColor(xAxis.mTitleColor);
        painter.setPen(pen);
        painter.setFont(xAxis.mTitleFont);

        double spaceInterval = (width() - leftPadding - rightPadding) / xAxis.mTitleCount;
        double valueInterval =( xAxis.mShowMax - xAxis.mShowMin) / xAxis.mTitleCount;

        QFontMetrics fm = painter.fontMetrics();
        for (int i=0;i<=xAxis.mTitleCount;i++) {
            QString title;
            if (xAxis.mTitleFormatter != nullptr) {
                title = xAxis.mTitleFormatter(xAxis.mShowMin + i*valueInterval);
            }
            else {
                title = QString("%1").arg(xAxis.mShowMin + i*valueInterval);
            }
            // 处理刻度值文本换行显示
            QStringList list = title.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

            /***************** 绘制文本水平居中显示 start ******************/
            double localY = 0;
            switch (xAxis.mTitlePosition) {
            case XAxisTitleTop:
                localY = topPadding - xAxis.mTitleAxisSpace - (list.size()-1)*(fm.ascent() + fm.descent());
                break;
            case XAxisTitleTopInner:
                localY = topPadding + xAxis.mTitleAxisSpace +  fm.ascent() + fm.descent();
                break;
            case XAxisTitleBottom:
                localY = height() - bottomPadding + xAxis.mTitleAxisSpace +  fm.ascent() + fm.descent();
                break;
            case XAxisTitleBottomInner:
                localY = height() - bottomPadding - xAxis.mTitleAxisSpace - (list.size()-1)*(fm.ascent() + fm.descent());
                break;
            }

            for (int j=0;j<list.size();j++) {
                QString subTitle = list[j];
                double titleWidth = fm.horizontalAdvance(subTitle);
                painter.drawText(QPointF(leftPadding + i*spaceInterval - titleWidth / 2,  localY + j*(fm.ascent() + fm.descent())), subTitle);
            }

            /***************** 绘制文本水平居中显示 end ******************/
        }
    }

    if (enableLeftAxis && leftAxis.mTitleEnable) {
        QPen pen;
        pen.setColor(leftAxis.mTitleColor);
        painter.setPen(pen);
        painter.setFont(leftAxis.mTitleFont);

        double spaceInterval = (height() - topPadding - bottomPadding) / leftAxis.mTitleCount;
        double valueInterval =( leftAxis.mShowMax - leftAxis.mShowMin) / leftAxis.mTitleCount;

        QFontMetrics fm = painter.fontMetrics();


        for (int i=0;i<=leftAxis.mTitleCount;i++) {
            QString title;
            if (leftAxis.mTitleFormatter != nullptr) {
                title = leftAxis.mTitleFormatter(leftAxis.mShowMax - i*valueInterval);
            }
            else {
                title = QString("%1").arg(leftAxis.mShowMax - i*valueInterval);
            }
            // 处理刻度值文本换行显示
            QStringList list = title.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

            double titleMaxWidth = 0;
            for (int j=0;j<list.size();j++) {
                QString subTitle = list[j];
                double titleWidth = fm.horizontalAdvance(subTitle);
                if (titleWidth > titleMaxWidth) {
                    titleMaxWidth = titleWidth;
                }
            }

            /***************** 绘制文本垂直居中显示 start ******************/
            double localCenterX = 0;
            switch (leftAxis.mTitlePosition) {
            case YAxisBaselineLeft:
                localCenterX = leftPadding - leftAxis.mTitleAxisSpace - titleMaxWidth / 2;
                break;
            case YAxisBaselineRight:
                localCenterX = leftPadding + leftAxis.mTitleAxisSpace + titleMaxWidth / 2;
                break;
            }

            double titleTotalHeight = fm.height() * list.size();
            for (int j=0;j<list.size();j++) {
                QString subTitle = list[j];
                double titleWidth = fm.horizontalAdvance(subTitle);

                double drawX = localCenterX-titleWidth/2;
                double drawY = topPadding + i*spaceInterval - titleTotalHeight / 2 +( j+1)*fm.height();
                painter.drawText(QPointF(drawX, drawY), subTitle);
            }
            /***************** 绘制文本垂直居中显示 end ******************/
        }
    }

    if (enableRightAxis && rightAxis.mTitleEnable) {
        QPen pen;
        pen.setColor(rightAxis.mTitleColor);
        painter.setPen(pen);
        painter.setFont(rightAxis.mTitleFont);

        double spaceInterval = (height() - topPadding - bottomPadding) / rightAxis.mTitleCount;
        double valueInterval =( rightAxis.mShowMax - rightAxis.mShowMin) / rightAxis.mTitleCount;

        QFontMetrics fm = painter.fontMetrics();

        for (int i=0;i<=rightAxis.mTitleCount;i++) {
            QString title;
            if (rightAxis.mTitleFormatter != nullptr) {
                title = rightAxis.mTitleFormatter(rightAxis.mShowMax - i*valueInterval);
            }
            else {
                title = QString("%1").arg(rightAxis.mShowMax - i*valueInterval);
            }
            QStringList list = title.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

            double titleMaxWidth = 0;
            for (int j=0;j<list.size();j++) {
                QString subTitle = list[j];
                double titleWidth = fm.horizontalAdvance(subTitle);
                if (titleWidth > titleMaxWidth) {
                    titleMaxWidth = titleWidth;
                }
            }

            /***************** 绘制文本垂直居中显示 start ******************/
            double localCenterX = 0;
            switch (rightAxis.mTitlePosition) {
            case YAxisBaselineLeft:
                localCenterX = width() - rightPadding - rightAxis.mTitleAxisSpace - titleMaxWidth / 2 ;
                break;
            case YAxisBaselineRight:
                localCenterX = width() - rightPadding + rightAxis.mTitleAxisSpace + titleMaxWidth / 2;
                break;
            }

            double titleTotalHeight = fm.height() * list.size();
            for (int j=0;j<list.size();j++) {
                QString subTitle = list[j];
                double titleWidth = fm.horizontalAdvance(subTitle);

                double drawX = localCenterX-titleWidth/2;
                double drawY = topPadding + i*spaceInterval - titleTotalHeight / 2 +( j+1)*fm.height();
                painter.drawText(QPointF(drawX, drawY), subTitle);
            }
             /***************** 绘制文本垂直居中显示 end ******************/
        }
    }
}

void QGLBaseChart::drawData()
{
    emit willShowXAxis(this->xAxis.mShowMin, this->xAxis.mShowMax);

    int length = dataSets.size();
    long long drawStart = QDateTime::currentDateTime().toMSecsSinceEpoch();

    for (int i=0;i<length;i++) {
        QGLDataSet *set = dataSets[i];
        drawSet(set);
    }
    long long drawEnd = QDateTime::currentDateTime().toMSecsSinceEpoch();
    qDebug() << "draw duration:" << drawEnd - drawStart;
}

void QGLBaseChart::drawSet(QGLDataSet *set)
{
    Q_UNUSED(set);
}

void QGLBaseChart::drawSelected()
{
    if (selectedRect.isEmpty()) {
        return;
    }

    // 选择区与数据区重叠，开启透明度混合模式
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(selectedColor.red() / 255.0f, selectedColor.green() / 255.0f, selectedColor.blue() / 255.0f, 0.8f);
    glBegin(GL_QUADS);

    glVertex3f(translateGLPositionX(this, xValuePosition(this, selectedRect.x())), translateGLPositionY(this, yValuePosition(this, selectedRect.y(), selectedDependy)), 0);
    glVertex3f(translateGLPositionX(this, xValuePosition(this, selectedRect.x()+selectedRect.width())), translateGLPositionY(this, yValuePosition(this, selectedRect.y(), selectedDependy)), 0);
    glVertex3f(translateGLPositionX(this, xValuePosition(this, selectedRect.x()+selectedRect.width())), translateGLPositionY(this, yValuePosition(this, selectedRect.y()+selectedRect.height(), selectedDependy)), 0);
    glVertex3f(translateGLPositionX(this, xValuePosition(this, selectedRect.x())), translateGLPositionY(this, yValuePosition(this, selectedRect.y()+selectedRect.height(), selectedDependy)), 0);

    glEnd();

    glDisable(GL_BLEND);
}

double QGLBaseChart::xValuePosition(QGLBaseChart *chart, double x)
{
    double contentWidth = chart->width() - chart->leftPadding - chart->rightPadding;
    double range = chart->xAxis.mShowMax - chart->xAxis.mShowMin;
    double ratio = fabs((x - chart->xAxis.mShowMin) / range);
    double valueWidth = contentWidth * ratio;

    if (x >= chart->xAxis.mShowMin) {
        return  chart->leftPadding + valueWidth;
    }
    else {
        return chart->leftPadding - valueWidth;
    }
}

double QGLBaseChart::yValuePosition(QGLBaseChart *chart, double y, AxisDependy denpendy)
{
    double contentHeight = chart->height() - chart->topPadding - chart->bottomPadding;
    double axisMin = denpendy == AxisDependyLeft ? chart->leftAxis.mShowMin : chart->rightAxis.mShowMin;
    double axisMax = denpendy == AxisDependyLeft ? chart->leftAxis.mShowMax : chart->rightAxis.mShowMax;

    double range = axisMax - axisMin;
    double ratio = fabs((y - axisMin) / range);
    double valueHeight = contentHeight * ratio;

    if (y >= axisMin) {
        return chart->height() - chart->bottomPadding - valueHeight;
    }
    else {
        return chart->height() - chart->bottomPadding + valueHeight;
    }
}

double QGLBaseChart::translateGLPositionX(QGLBaseChart *chart, double xPosition)
{
    double ratio = xPosition / chart->width();
    return ratio * 2 - 1;
}

double QGLBaseChart::translateGLPositionY(QGLBaseChart *chart, double yPosition)
{
    double ratio = yPosition / chart->height();

    if (yPosition < 0) {
        return fabs(ratio * 2) + 1;
    }
    else {
        return 1 - ratio * 2;
    }
}
