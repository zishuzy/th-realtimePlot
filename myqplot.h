#ifndef MYQPLOT_H
#define MYQPLOT_H

#include "qcustomplot.h"
#include <QDebug>
#include <QPoint>
#include <QVector>
#include <QWidget>

// 最大线
#define MAX_ARG (10)

class MyQPlot : public QCustomPlot
{
    Q_OBJECT

public:
    MyQPlot(QWidget *parent = 0);
    virtual ~MyQPlot();

    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void addGraph();
    int getPlotCount();
    void myClear();
    void mySetData(const int plot, const QVector<double> &keys, const QVector<double> &values);
    void myAddData(const int plot, const double key, double value);
    void setHide(const int &plot);
    void setShow(const int &plot);

    void set_xAxisMax(const uint32_t xmax);

private:
    void auto_axis_x();
    void auto_axis_y();

private:
    bool m_ctrlPush;
    int m_plotsCount;
    QVector<bool> m_plotShow;
    QVector<QVector<double>> m_graph_x;
    QVector<QVector<double>> m_graph_y;
    bool m_shiftFlag;
    double m_maxGraph_y;
    double m_minGraph_y;
    int m_lastx, m_lasty;
    uint32_t m_xCountMax;
};

#endif // MYQPLOT_H
