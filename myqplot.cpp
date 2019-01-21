#include "myqplot.h"

MyQPlot::MyQPlot(QWidget *parent) : QCustomPlot(parent), m_graph_x(MAX_ARG), m_graph_y(MAX_ARG), m_plotShow(MAX_ARG)
{
    m_ctrlPush = false;
    for (int i = 0; i < MAX_ARG; i++) {
        m_plotShow[i] = true;
    }
    m_plotsCount = 0;
    m_shiftFlag = false;
    m_maxGraph_y = -1;
    m_minGraph_y = 1;
    m_lastx = 0;
    m_lasty = 0;
    m_xCountMax = 0;
}

MyQPlot::~MyQPlot() {}

//鼠标移动
void MyQPlot::mouseMoveEvent(QMouseEvent *event)
{
    // 这里必须使用buttons()
    if (event->buttons() & Qt::LeftButton) { //进行的按位与
        QCustomPlot::mouseMove(event);
        if (m_lastx > 0) {
            double lower, upper;
            double err;
            int vsize;
            vsize = this->viewport().size().width();
            lower = this->xAxis->range().lower;
            upper = this->xAxis->range().upper;
            err = 1.1 * (m_lastx - event->x()) / vsize * (upper - lower);
            this->xAxis->setRange(lower + err, upper + err);
            vsize = this->viewport().size().height();
            lower = this->yAxis->range().lower;
            upper = this->yAxis->range().upper;
            err = 1.1 * (m_lasty - event->y()) / vsize * (upper - lower);
            this->yAxis->setRange(lower - err, upper - err);
            // lower = this->yAxis2->range().lower;
            // upper = this->yAxis2->range().upper;
            // err = 1.1 * (m_lasty - y) / vsize * (upper - lower);
            // this->yAxis2->setRange(lower - err, upper - err);
            this->replot();
        }
        m_lastx = event->x();
        m_lasty = event->y();
    }
}
//滚轮事件
void MyQPlot::wheelEvent(QWheelEvent *event)
{
    double lower, upper;
    double err;
    int delta = event->delta();
    if (m_shiftFlag) {
        lower = this->yAxis->range().lower;
        upper = this->yAxis->range().upper;
        err = 1.1 * (-delta) / 1000.0 * (upper - lower);
        this->yAxis->setRange(lower - err, upper + err);
    } else {

        lower = this->xAxis->range().lower;
        upper = this->xAxis->range().upper;
        err = 1.1 * (-delta) / 1000.0 * (upper - lower);
        this->xAxis->setRange(lower - err, upper + err);
    }
    this->replot();
}
//鼠标释放
void MyQPlot::mouseReleaseEvent(QMouseEvent *event)
{
    m_lastx = m_lasty = -1;
}
// 键盘按下事件
void MyQPlot::keyPressEvent(QKeyEvent *event)
{
    // 是否按下Shift键      特殊按键
    if (event->modifiers() == Qt::ShiftModifier) {
        m_shiftFlag = true;
    }
}
// 键盘释放事件
void MyQPlot::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift) {
        m_shiftFlag = false;
    }
}
// 添加线
void MyQPlot::addGraph()
{
    m_plotsCount++;
    this->QCustomPlot::addGraph();
}
// 返回曲线个数
int MyQPlot::getPlotCount()
{
    return m_plotsCount;
}
// 清空线
void MyQPlot::myClear()
{
    for (int i = 0; i < MAX_ARG && i < m_plotsCount; i++) {
        m_graph_x[i].clear();
        m_graph_y[i].clear();
        this->graph(i)->data().data()->clear();
    }
    m_maxGraph_y = -1;
    m_minGraph_y = 1;
    this->replot();
}
// 设置数据
void MyQPlot::mySetData(const int plot, const QVector<double> &keys, const QVector<double> &values)
{
    m_graph_x[plot] = keys;
    m_graph_y[plot] = values;
    if (m_plotShow.at(plot)) {
        this->graph(plot)->setData(keys, values);
    }
}
// 添加数据
void MyQPlot::myAddData(const int plot, const double key, const double value)
{
//   m_graph_x[plot].append(key);
//   m_graph_y[plot].append(value);
    if (m_plotShow.at(plot)) {
        if (value > m_maxGraph_y) {
            m_maxGraph_y = value;
        } else if (value < m_minGraph_y) {
            m_minGraph_y = value;
        }
        this->graph(plot)->addData(key, value);
        if (key < m_xCountMax)
            this->xAxis->setRange(0, key);
        else
            this->xAxis->setRange(key - m_xCountMax, key);
        this->yAxis->setRange(m_minGraph_y, m_maxGraph_y);
    }
}
// 隐藏
void MyQPlot::setHide(const int &plot)
{
    m_plotShow[plot] = false;
    this->graph(plot)->data().data()->clear();
    this->replot();
}
// 显示
void MyQPlot::setShow(const int &plot)
{
    m_plotShow[plot] = true;
    this->graph(plot)->setData(m_graph_x[plot], m_graph_y[plot]);
    this->replot();
}
// 设置x轴最大显示数
void MyQPlot::set_xAxisMax(const uint32_t xmax)
{
    m_xCountMax = xmax;
}
