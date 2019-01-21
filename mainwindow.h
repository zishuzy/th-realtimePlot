#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "datacollectionthread.h"
#include <QMainWindow>
#include <QCloseEvent>
#include <QVector>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);

    void init_port();
    void init_plot();
    void sys_start();
    void sys_stop();

signals:
    void set_parameters_signal(const QString &port, int baud);
    void startOrStop_signal(bool);

private slots:
    void fileOpenErrorSlot();
    void get_dataFromThreadSlot(double value);
    void on_btn_startStop_clicked();

    void on_btn_xAxis_ok_clicked();

private:
    Ui::MainWindow *ui;
    DataCollectionThread *m_pCollectThread;
    QVector<double>  m_plotKeys;
    QVector<double>  m_plotValues;
    double m_plotKey;
    double m_plotValue;
};

#endif // MAINWINDOW_H
