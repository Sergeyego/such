#include "mblbl.h"

MbLbl::MbLbl(QLabel *lab, QString p0, QString p1, int slaveAdr, int b, QModbusClient *d, QObject *parent) : label(lab), pic0(p0), pic1(p1), slave(slaveAdr), bit(b), device(d), QObject(parent)
{
    old_status=false;
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(sendReadRequest()));
    timer->start();
}

void MbLbl::sendReadRequest()
{
    if (!device)
        return;
    if (device->state()==QModbusDevice::ConnectedState) {
        QModbusDataUnit readRequest(QModbusDataUnit::Coils,bit,1);
        if (auto reply = device->sendReadRequest(readRequest,slave)){
            if (!reply->isFinished()) {
                connect(reply, &QModbusReply::finished, this, &MbLbl::readyRead);
            } else {
                delete reply;
            }
        }
    }
}

void MbLbl::readyRead()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;
    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        reply->deleteLater();
        int rc=unit.valueCount();
        if (rc==1){
            bool st=(unit.value(0)!=0);
            if (!st){
                if (label->pixmap()!=&pic0) label->setPixmap(pic0);
            } else {
                if (label->pixmap()!=&pic1) label->setPixmap(pic1);
            }
            /*if (st!= old_status) */
            emit newStatus(st);
            old_status=st;
        } else {
            qDebug()<<"read error";
        }
    }
}
