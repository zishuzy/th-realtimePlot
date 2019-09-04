#include "datacollectionthread.h"
#include "some_fun.h"
#include <QDateTime>
#include <QDebug>
#include <QDir>

DataCollectionThread::DataCollectionThread(QObject *parent) : m_pSerialPort(nullptr), m_pFileLog(nullptr)
{
    this->moveToThread(this);
}

DataCollectionThread::~DataCollectionThread()
{
    requestInterruption();
    quit();
    wait();

    SAFE_DELETE(m_pSerialPort);
    if (m_pFileLog->isOpen()) {
        m_pFileLog->close();
    }
    SAFE_DELETE(m_pFileLog);
}

void DataCollectionThread::run()
{
    m_runFlag = false;
    if (m_pSerialPort == nullptr) {
        m_pSerialPort = new QSerialPort();
    }
    if (m_pFileLog == nullptr) {
        m_pFileLog = new QFile();
    }
    init_logDir();
    init_baudMap();
    connect(m_pSerialPort, &QSerialPort::readyRead, this, &DataCollectionThread::readSerial);
    while (!isInterruptionRequested()) {
        //        qDebug() << "while";
        this->exec();
    }
}

void DataCollectionThread::init_logDir()
{
    QDir logDir("./log");
    if (!logDir.exists()) {
        QDir t;
        t.mkdir("./log");
    }
}

void DataCollectionThread::init_baudMap()
{
    m_baudMap.insert(1200, QSerialPort::Baud1200);
    m_baudMap.insert(2400, QSerialPort::Baud2400);
    m_baudMap.insert(4800, QSerialPort::Baud4800);
    m_baudMap.insert(9600, QSerialPort::Baud9600);
    m_baudMap.insert(19200, QSerialPort::Baud19200);
    m_baudMap.insert(38400, QSerialPort::Baud38400);
    m_baudMap.insert(57600, QSerialPort::Baud57600);
    m_baudMap.insert(115200, QSerialPort::Baud115200);
}

bool DataCollectionThread::get_dataFromMsg(double *outData)
{
    bool okflag;
    if (m_receiveData.isEmpty()) {
        okflag = false;
    } else {
        int len = m_receiveData.size();
        QString dataStr;
        for (int i = 0; i < len; ++i) {
            if (m_receiveData[i] == '\n') {
                dataStr = m_receiveData.mid(0, i + 1);
                *outData = dataStr.split(" ")[0].toDouble();
                m_receiveData.chop(i + 1);
                okflag = true;
            }
        }
    }
    return okflag;
}

/**************** private slots *****************************/

void DataCollectionThread::readSerial()
{
    //qDebug() << QThread::currentThreadId();
    QByteArray tmpmsg = m_pSerialPort->readLine();
    if (!m_runFlag)
        return;
    m_receiveData += tmpmsg;
    QTextStream out(m_pFileLog);
    // out << m_receiveData << endl;
    out << tmpmsg;
    out.flush();
    double tmpdata;
    bool okflag;
    okflag = get_dataFromMsg(&tmpdata);
    if (okflag) {
        emit this->dataFromMsg(tmpdata);
    }
    //this->exit();
}

/**************** public slots *****************************/

void DataCollectionThread::set_parameters_slot(const QString &port, int baud)
{
    m_pSerialPort->setPortName(port);
    m_pSerialPort->setBaudRate(m_baudMap.value(baud));
    m_pSerialPort->setDataBits(QSerialPort::Data8);
    m_pSerialPort->setParity(QSerialPort::NoParity);
    m_pSerialPort->setStopBits(QSerialPort::OneStop);
    m_pSerialPort->setFlowControl(QSerialPort::NoFlowControl);
    //this->exit();
}

void DataCollectionThread::set_startOrStop_slot(bool status)
{
    m_runFlag = status;
    if (m_runFlag) {
        QString cuDTstr = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
        m_pFileLog->setFileName(QString("./log/%1.log").arg(cuDTstr));
        if (m_pFileLog->open(QIODevice::WriteOnly | QIODevice::Text)) {
            if (m_pSerialPort->open(QIODevice::ReadWrite)) {
                qDebug() << "成功";
                // 成功
                return;
            }
        }
        // 失败了
        emit this->fileOpenError();
        m_runFlag = false;
    } else {
        if (m_pFileLog->isOpen()) {
            m_pFileLog->close();
        }
        if (m_pSerialPort->isOpen()) {
            m_pSerialPort->close();
        }
    }
    //this->exit();
}
