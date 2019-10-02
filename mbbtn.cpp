#include "mbbtn.h"


MbBtn::MbBtn(QPushButton *btn, int slaveAdr, int b, QModbusClient *d, QString mess, QObject *parent) : slave(slaveAdr), bit(b), device(d), message(mess), QObject(parent)
{
    timer = new QTimer(this);
    timer->setInterval(500);
    connect(btn,SIGNAL(clicked(bool)),this,SLOT(trig()));
    connect(timer,SIGNAL(timeout()),this,SLOT(setOff()));
}

void MbBtn::trig()
{
    if (!device)
        return;
    if (device->state()==QModbusDevice::ConnectedState) {
        int n;
        if (!message.isEmpty()){
            n = QMessageBox::question(NULL,QString::fromUtf8("Внимание!"),message,QMessageBox::Yes,QMessageBox::No);
        }
        if (message.isEmpty() || n==QMessageBox::Yes){
            QModbusDataUnit writeRequest(QModbusDataUnit::Coils,bit,1);
            writeRequest.setValue(0,1);
            if (auto *reply = device->sendWriteRequest(writeRequest,slave)) {
                if (!reply->isFinished()) {
                    timer->start();
                    connect(reply, &QModbusReply::finished, this, [this, reply]() {
                        if (reply->error()!=QModbusDevice::NoError){
                            qDebug()<<reply->errorString();
                        } else {
                            emit wrOk();
                        }
                        reply->deleteLater();
                    });
                } else {
                    reply->deleteLater();
                }
            }
        }
    }
}

void MbBtn::setOff()
{
    if (!device)
        return;
    timer->stop();
    if (device->state()==QModbusDevice::ConnectedState) {
        QModbusDataUnit writeRequest(QModbusDataUnit::Coils,bit,1);
        writeRequest.setValue(0,0);
        if (auto *reply = device->sendWriteRequest(writeRequest,slave)) {
            if (!reply->isFinished()) {
                connect(reply, &QModbusReply::finished, this, [this, reply]() {
                    if (reply->error()!=QModbusDevice::NoError){
                        qDebug()<<reply->errorString();
                    }
                    reply->deleteLater();
                });
            } else {
                reply->deleteLater();
            }
        }
    }
}
