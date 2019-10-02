#ifndef REGS_H
#define REGS_H

#include <QObject>
#include <QModbusDevice>
#include <QModbusClient>
#include <QModbusDataUnit>
#include <QModbusReply>
#include <QTimer>
#include <QDebug>

typedef QVector <quint16> regData;

class Regs : public QObject
{
    Q_OBJECT
public:
    Regs(int slaveAdr, int reg, int n, int t, QModbusClient *d, QObject *parent=0);

private:
    int kvo;
    int adr;
    QModbusClient *device;
    int slave;
    QTimer *timer;

private slots:
    void readyRead();

public slots:
    void writeRegs(regData u);
    void writeRegs(int shift, regData u);
    void sendReadRequest();

signals:
    void newVal(regData u);
};

#endif // REGS_H
