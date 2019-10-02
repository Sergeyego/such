#ifndef MBBTN_H
#define MBBTN_H

#include <QObject>
#include <QModbusDevice>
#include <QModbusClient>
#include <QModbusDataUnit>
#include <QModbusReply>
#include <QTimer>
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>

class MbBtn : public QObject
{
    Q_OBJECT
public:
    explicit MbBtn(QPushButton *btn, int slaveAdr, int b, QModbusClient *d, QString mess, QObject *parent = 0);

private:
    QModbusClient *device;
    int slave;
    int bit;
    QTimer *timer;
    QString message;

private slots:
    void trig();
    void setOff();
signals:
    void wrOk();

public slots:
};

#endif // MBBTN_H
