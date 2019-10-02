#include "regs.h"

Regs::Regs(int slaveAdr, int reg, int n, int t, QModbusClient *d, QObject *parent) : slave(slaveAdr), adr(reg), kvo(n), device(d), QObject(parent)
{
    timer = new QTimer(this);
    timer->setInterval(t);
    connect(timer,SIGNAL(timeout()),this,SLOT(sendReadRequest()));
    timer->start();
}

void Regs::sendReadRequest()
{
    if (!device)
        return;
    if (device->state()==QModbusDevice::ConnectedState) {
        QModbusDataUnit readRequest(QModbusDataUnit::HoldingRegisters,adr,kvo);
        if (auto reply = device->sendReadRequest(readRequest,slave)){
            if (!reply->isFinished()) {
                connect(reply, &QModbusReply::finished, this, &Regs::readyRead);
            } else {
                delete reply;
            }
        }
    }
}

void Regs::readyRead()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;
    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        reply->deleteLater();
        int rc=unit.valueCount();
        if (rc==kvo){
            regData d;
            for (int i=0; i<unit.valueCount(); i++){
                d.push_back(unit.value(i));
            }
            emit newVal(d);
        } else {
            regData dn;
            dn.resize(kvo);
            dn.fill(0);
            emit newVal(dn);
            qDebug()<<"read error: "<<reply->errorString();
        }        
    }
}

void Regs::writeRegs(regData u)
{
    return writeRegs(0,u);
}

void Regs::writeRegs(int shift, regData u)
{
    if (!device)
        return;
    if (device->state()==QModbusDevice::ConnectedState) {
        QModbusDataUnit writeRequest(QModbusDataUnit::HoldingRegisters,adr+shift,u.size());
        for (int i=0; i<u.size(); i++){
            writeRequest.setValue(i,u.at(i));
        }
        if (auto *reply = device->sendWriteRequest(writeRequest,slave)) {
            if (!reply->isFinished()) {
                connect(reply, &QModbusReply::finished, this, [this, reply]() {
                    if (reply->error()!=QModbusDevice::NoError){
                        qDebug()<<reply->errorString();
                    }
                    reply->deleteLater();
                });
            } else {
                qDebug()<<"write error: "<<reply->errorString();
                reply->deleteLater();
            }
        }
    }
}
