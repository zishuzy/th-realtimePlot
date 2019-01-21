#include "mainwindow.h"
#include "some_fun.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <float.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("大哥翟宇的牛逼之作v0.1.1");
    //qDebug() << QThread::currentThreadId();
    m_pCollectThread = new DataCollectionThread();
    connect(m_pCollectThread, &DataCollectionThread::fileOpenError, this, &MainWindow::fileOpenErrorSlot);
    connect(this, &MainWindow::set_parameters_signal, m_pCollectThread, &DataCollectionThread::set_parameters_slot);
    connect(this, &MainWindow::startOrStop_signal, m_pCollectThread, &DataCollectionThread::set_startOrStop_slot);
    connect(m_pCollectThread, &DataCollectionThread::dataFromMsg, this, &MainWindow::get_dataFromThreadSlot);
    m_pCollectThread->start();
    init_port();
    init_plot();
}

MainWindow::~MainWindow()
{
    SAFE_DELETE(m_pCollectThread);
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    sys_stop();
}

void MainWindow::init_port()
{
    ui->comboBox_port->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite)) {
            ui->comboBox_port->addItem(info.portName());
            serial.close();
        }
    }
}

void MainWindow::init_plot()
{
    // 初始化信号
    // connect(ui->widget_plot, &MyQPlot::mouseMove, this, &MainWindow::mouseMoveSlot);
    // connect(ui->widget_plot, &MyQPlot::mouseRelease, this, &MainWindow::mouseReleseSlot);
    // connect(ui->widget_plot, &MyQPlot::mouseWheel, this, &MainWindow::wheelSlot);

    ui->widget_plot->addGraph();
    // 网格
    ui->widget_plot->xAxis->grid()->setVisible(false);

    ui->widget_plot->rescaleAxes(true);
    // ui->widget_plot->yAxis->setRange(0,0.0008);
}

void MainWindow::sys_start()
{
    emit this->set_parameters_signal(ui->comboBox_port->currentText(), ui->comboBox_baud->currentText().toInt());
    emit this->startOrStop_signal(true);
    ui->btn_startStop->setText("停止");
    ui->comboBox_port->setEnabled(false);
    ui->comboBox_baud->setEnabled(false);
    m_plotKey = 0;
    ui->widget_plot->myClear();
    on_btn_xAxis_ok_clicked();
}

void MainWindow::sys_stop()
{
    emit this->startOrStop_signal(false);
    ui->btn_startStop->setText("开始");
    ui->comboBox_port->setEnabled(true);
    ui->comboBox_baud->setEnabled(true);
}

void MainWindow::on_btn_startStop_clicked()
{
    if (ui->btn_startStop->text() == "开始") {
        sys_start();
    } else {
        sys_stop();
    }
}

void MainWindow::on_btn_xAxis_ok_clicked()
{
    ui->widget_plot->set_xAxisMax(ui->lineEdit_xAxisMax->text().toUInt());
}
/**************** public slots *****************************/

void MainWindow::fileOpenErrorSlot()
{
    qDebug() << "error";
    sys_stop();
}

void MainWindow::get_dataFromThreadSlot(double value)
{
    m_plotValue = value;
    ui->widget_plot->myAddData(0, m_plotKey++, m_plotValue);
    ui->widget_plot->replot();
}


