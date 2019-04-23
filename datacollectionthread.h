#ifndef DATACOLLECTIONTHREAD_H
#define DATACOLLECTIONTHREAD_H

#include <QFile>
#include <QMap>
#include <QTextStream>
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class DataCollectionThread : public QThread
{
    Q_OBJECT
public:
    explicit DataCollectionThread(QObject *parent = nullptr);
    ~DataCollectionThread();

    virtual void run();

private:
    void init_logDir();
    void init_baudMap();
    bool get_dataFromMsg(double *outData);

signals:
    void fileOpenError();
    void dataFromMsg(double value);

private slots:
    void readSerial();

public slots:
    void set_parameters_slot(const QString &port, int baud);
    void set_startOrStop_slot(bool status);

private:
    QSerialPort *m_pSerialPort;
    bool m_runFlag;
    QByteArray m_receiveData;
    QFile *m_pFileLog;
    QMap<int, QSerialPort::BaudRate> m_baudMap;
};

#endif // DATACOLLECTIONTHREAD_H
