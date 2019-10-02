#ifndef MBLBL_H
#define MBLBL_H

#include <QObject>
#include <QModbusDevice>
#include <QModbusClient>
#include <QModbusDataUnit>
#include <QModbusReply>
#include <QTimer>
#include <QDebug>
#include <QLabel>

class MbLbl : public QObject
{
    Q_OBJECT
public:
    explicit MbLbl(QLabel *lab, QString p0, QString p1, int slaveAdr, int b, QModbusClient *d, QObject *parent = 0);
private:
    QModbusClient *device;
    QLabel *label;
    int slave;
    int bit;
    QPixmap pic0;
    QPixmap pic1;
    QTimer *timer;
    bool old_status;

private slots:
    void readyRead();

signals:
    void newStatus(bool s);
public slots:
    void sendReadRequest();
};

#endif // MBLBL_H
