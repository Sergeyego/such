#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(int interval, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadSettings();
    ui->tabWidget->setStyleSheet(QString("QTabBar::tab { padding: %1px }").arg(10));

    errMessageBox = new QMessageBox(this);
    errMessageBox->setModal(false);
    errMessageBox->setWindowTitle(QString::fromUtf8("Ошибка"));
    errMessageBox->setIcon(QMessageBox::Critical);

    modbusDevice=nullptr;
    mD1=nullptr;
    mD2=nullptr;
    connectDev();

    mbStart = new MbBtn(ui->cmdStart,1,2148,modbusDevice, QString::fromUtf8("Подтвердите выполнение опасного действия! Вы уверены, что хотите запустить вентиляторы?"),this);
    mbStop = new MbBtn(ui->cmdStop,1,2149,modbusDevice,QString::fromUtf8("Вы уверены, что хотите остановить вентиляторы?"),this);
    mbReset = new MbBtn(ui->cmdReset,1,2150,modbusDevice,QString(),this);
    mbTechStart = new MbBtn(ui->cmdTechStart,1,2153,modbusDevice,QString(),this);
    mbTechStop = new MbBtn(ui->cmdTechStop,1,2154,modbusDevice,QString::fromUtf8("Остановить программу технолога?"),this);

    mbLblRun = new MbLbl(ui->labelRun,":/off.png",":/green.png",1,1283,modbusDevice,this);
    mbLblReady = new MbLbl(ui->labelReady,":/off.png",":/orange.png",1,1282,modbusDevice,this);
    mbLblAlarm = new MbLbl(ui->labelError,":/off.png",":/red.png",1,1281,modbusDevice,this);
    mbLblM1 = new MbLbl(ui->labelM1,":/off.png",":/green.png",1,2089,modbusDevice,this);
    mbLblM2 = new MbLbl(ui->labelM2,":/off.png",":/green.png",1,2090,modbusDevice,this);
    mbLblM3 = new MbLbl(ui->labelM3,":/off.png",":/green.png",1,2091,modbusDevice,this);
    mbLblM4 = new MbLbl(ui->labelM4,":/off.png",":/green.png",1,2092,modbusDevice,this);
    mbLblM5 = new MbLbl(ui->labelM5,":/off.png",":/green.png",1,2093,modbusDevice,this);
    mbLblM6 = new MbLbl(ui->labelM6,":/off.png",":/green.png",1,2094,modbusDevice,this);
    mbLblM7 = new MbLbl(ui->labelM7,":/off.png",":/green.png",1,2095,modbusDevice,this);
    mbLblM8 = new MbLbl(ui->labelM8,":/off.png",":/green.png",1,2096,modbusDevice,this);
    mbLblM9 = new MbLbl(ui->labelM9,":/off.png",":/green.png",1,2097,modbusDevice,this);

    mbLblTech = new MbLbl(ui->labelTech,":/off.png",":/green.png",1,2098,modbusDevice,this);

    regsCount = new Regs(1,1840,4,1000,modbusDevice,this);
    regsTechStatus = new Regs(1,4112,4,1000,modbusDevice,this);

    ui->horizontalLayoutPlot->addWidget(Plot::instance());
    cfgPlot = new CfgPlot(Plot::instance(),this);
    ui->verticalLayoutCfg->addWidget(cfgPlot);

    countPlot=0;
    timerPlot = new QTimer(this);
    timerPlot->setInterval(interval*1000);
    Plot::instance()->setbaseTime(QDateTime::currentDateTime());
    Plot::instance()->setInterval(interval);

    timerCon = new QTimer(this);
    timerCon->setInterval(2000);

    modelZone1 = new ModelChannel(4296,4300,4506,4136, modbusDevice, this);
    modelZone1->setData(modelZone1->index(0,1),QColor(255,0,0));
    modelZone1->setData(modelZone1->index(1,1),QColor(0,255,0));
    modelZone1->setData(modelZone1->index(3,1),QColor(0,0,255));
    ui->tableViewZone1->setModel(modelZone1);
    ui->tableViewZone1->setColumnWidth(0,100);
    ui->tableViewZone1->setColumnWidth(1,30);
    ui->tableViewZone1->setColumnWidth(2,100);
    ui->tableViewZone1->setItemDelegateForColumn(1, new ColorDelegate(this));
    ui->tableViewZone1->setItemDelegateForColumn(2, new LineColorDelegate(this));

    modelZone2 = new ModelChannel(4297,4301,4507,4137, modbusDevice, this);
    modelZone2->setData(modelZone2->index(0,1),QColor(245,50,200));
    modelZone2->setData(modelZone2->index(1,1),QColor(60,200,160));
    modelZone2->setData(modelZone2->index(3,1),QColor(100,90,200));
    ui->tableViewZone2->setModel(modelZone2);
    ui->tableViewZone2->setColumnWidth(0,100);
    ui->tableViewZone2->setColumnWidth(1,30);
    ui->tableViewZone2->setColumnWidth(2,100);
    ui->tableViewZone2->setItemDelegateForColumn(1, new ColorDelegate(this));
    ui->tableViewZone2->setItemDelegateForColumn(2, new LineColorDelegate(this));

    modelZone3 = new ModelChannel(4298,4302,4508,4138, modbusDevice, this);
    modelZone3->setData(modelZone3->index(0,1),QColor(255,90,0));
    modelZone3->setData(modelZone3->index(1,1),QColor(255,255,0));
    modelZone3->setData(modelZone3->index(3,1),QColor(200,90,200));
    ui->tableViewZone3->setModel(modelZone3);
    ui->tableViewZone3->setColumnWidth(0,100);
    ui->tableViewZone3->setColumnWidth(1,30);
    ui->tableViewZone3->setColumnWidth(2,100);
    ui->tableViewZone3->setItemDelegateForColumn(1, new ColorDelegate(this));
    ui->tableViewZone3->setItemDelegateForColumn(2, new LineColorDelegate(this));

    modelZone4 = new ModelChannel(4299,4303,4509,4139, modbusDevice, this);
    modelZone4->setData(modelZone4->index(0,1),QColor(255,120,150));
    modelZone4->setData(modelZone4->index(1,1),QColor(0,85,0));
    modelZone4->setData(modelZone4->index(3,1),QColor(85,85,120));
    ui->tableViewZone4->setModel(modelZone4);
    ui->tableViewZone4->setColumnWidth(0,100);
    ui->tableViewZone4->setColumnWidth(1,30);
    ui->tableViewZone4->setColumnWidth(2,100);
    ui->tableViewZone4->setItemDelegateForColumn(1, new ColorDelegate(this));
    ui->tableViewZone4->setItemDelegateForColumn(2, new LineColorDelegate(this));

    modelAlarm = new ModelAlarm(1,4106,2,modbusDevice,this);
    ui->tableViewAlarm->setModel(modelAlarm);
    ui->tableViewAlarm->setColumnWidth(0,800);

    modelTech = new ModelTech(1,4515,modbusDevice,this);
    ui->tableViewTech->setModel(modelTech);
    ui->tableViewTech->setItemDelegate(new LineDelegate(this));

    connect(timerPlot,SIGNAL(timeout()),this,SLOT(replot()));
    connect(regsCount,SIGNAL(newVal(regData)),this,SLOT(newCounter(regData)));
    connect(regsTechStatus,SIGNAL(newVal(regData)),this,SLOT(newTechStatus(regData)));
    connect(ui->cmdExit,SIGNAL(clicked(bool)),this,SLOT(exitPrg()));
    connect(ui->cmdUst,SIGNAL(clicked(bool)),this,SLOT(setAllUst()));
    connect(modelTech,SIGNAL(newStepCount(int)),this,SLOT(newStepCount(int)));
    connect(ui->cmdGoStep,SIGNAL(clicked(bool)),this,SLOT(setStepCount()));
    connect(mbLblTech,SIGNAL(newStatus(bool)),ui->cmdUst,SLOT(setDisabled(bool)));
    connect(mbLblTech,SIGNAL(newStatus(bool)),ui->cmdTechStart,SLOT(setDisabled(bool)));
    connect(mbLblTech,SIGNAL(newStatus(bool)),ui->cmdTechStop,SLOT(setEnabled(bool)));
    connect(mbTechStart,SIGNAL(wrOk()),mbLblTech,SLOT(sendReadRequest()));
    connect(mbTechStop,SIGNAL(wrOk()),mbLblTech,SLOT(sendReadRequest()));
    timerPlot->start();

    connect(timerCon,SIGNAL(timeout()),this,SLOT(conCheck()));
    timerCon->start();
}

MainWindow::~MainWindow()
{
    if (modbusDevice) modbusDevice->disconnectDevice();
    saveSettings();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->showMinimized();
}

void MainWindow::loadSettings()
{
    QSettings settings("szsm", "such");
    this->restoreGeometry(settings.value("such_geometry").toByteArray());
}

void MainWindow::saveSettings()
{
    QSettings settings("szsm", "such");
    settings.setValue("such_geometry", this->saveGeometry());
}

void MainWindow::replot()
{
    bool b=(countPlot>nMax);
    int xMin = b? countPlot-nMax : 0;
    modelZone1->plot(countPlot,b);
    modelZone2->plot(countPlot,b);
    modelZone3->plot(countPlot,b);
    modelZone4->plot(countPlot,b);
    Plot::instance()->setXscale(xMin,countPlot);
    countPlot++;
}

void MainWindow::connectStateChanged(QModbusDevice::State state)
{
    if (state==QModbusDevice::ConnectedState){
        ui->statusBar->showMessage(QString::fromUtf8("Подключено"));
    } else {
        ui->statusBar->showMessage(QString::fromUtf8("Не подключено"));
    }
}

void MainWindow::connectDev()
{
    //mutex.lock();
    if (modbusDevice) {
        if (mD1){
            if (modbusDevice==mD1){
                mD2 = new QModbusTcpClient(this);
                mD2->setConnectionParameter(QModbusDevice::NetworkPortParameter, 502);
                mD2->setConnectionParameter(QModbusDevice::NetworkAddressParameter, "192.168.1.5");
                mD2->setTimeout(500);
                mD2->setNumberOfRetries(2);
                modbusDevice=mD2;
                mD1->disconnectDevice();
                delete mD1;
                mD1=nullptr;
                //qDebug()<<"md1";
            }
        }
        if (mD2){
            if (modbusDevice==mD2){
                mD1 = new QModbusTcpClient(this);
                mD1->setConnectionParameter(QModbusDevice::NetworkPortParameter, 502);
                mD1->setConnectionParameter(QModbusDevice::NetworkAddressParameter, "192.168.1.5");
                mD1->setTimeout(500);
                mD1->setNumberOfRetries(2);
                modbusDevice=mD1;
                mD2->disconnectDevice();
                delete mD2;
                mD2=nullptr;
                //qDebug()<<"md2";
            }
        }
        //delete modbusDevice;
        //modbusDevice=nullptr;
    } else {
        mD1 = new QModbusTcpClient(this);
        mD1->setConnectionParameter(QModbusDevice::NetworkPortParameter, 502);
        mD1->setConnectionParameter(QModbusDevice::NetworkAddressParameter, "192.168.1.5");
        mD1->setTimeout(500);
        mD1->setNumberOfRetries(2);
        modbusDevice=mD1;
        //qDebug()<<"nullptr";
    }
    //modbusDevice = new QModbusTcpClient(this);
    /*modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, 502);
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, "192.168.1.5");
    modbusDevice->setTimeout(500);
    modbusDevice->setNumberOfRetries(2);*/
    connect(modbusDevice,SIGNAL(stateChanged(QModbusDevice::State)),this,SLOT(connectStateChanged(QModbusDevice::State)));
    connect(modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error) {
        showErrMessage(tr("Ошибка связи с контроллером: ")+modbusDevice->errorString());
    });
    modbusDevice->connectDevice();
    //mutex.unlock();
}

void MainWindow::showErrMessage(QString mess)
{
    errMessageBox->setText(QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss")+": "+mess);
    errMessageBox->show();
}

void MainWindow::newCounter(regData d)
{
    qint32 t;
    qint32 h;
    quint16 regt[2];
    quint16 regh[2];
    regt[0]=d.value(0);
    regt[1]=d.value(1);
    regh[0]=d.value(2);
    regh[1]=d.value(3);
    memcpy(((quint8*)&t)+0, regt+0, 2);
    memcpy(((quint8*)&t)+2, regt+1, 2);
    memcpy(((quint8*)&h)+0, regh+0, 2);
    memcpy(((quint8*)&h)+2, regh+1, 2);
    ui->labelCountT->setText(QString::number(t));
    ui->labelCountH->setText(QString::number(h));
}

void MainWindow::newTechStatus(regData d)
{
    ui->labelStep->setText(QString::number((qint16)d.at(0)));
    ui->labelStepTime->setText(QString::number((qint16)d.at(1)));
    ui->labelPr->setText(QString::number((qint16)d.at(2)));
    ui->labelOst->setText(QString::number((qint16)d.at(3)));
}

void MainWindow::exitPrg()
{
    int n=QMessageBox::question(this,tr("Выход"),tr("Завершить программу?"),QMessageBox::Yes,QMessageBox::No);
    if (n==QMessageBox::Yes) {
        QApplication::exit();
    }
}

void MainWindow::setAllUst()
{
    double val=ui->lineEditUst->text().toDouble();
    modelZone1->setData(modelZone1->index(1,2),val);
    modelZone2->setData(modelZone2->index(1,2),val);
    modelZone3->setData(modelZone3->index(1,2),val);
    modelZone4->setData(modelZone4->index(1,2),val);
}

void MainWindow::newStepCount(int n)
{
    ui->lineEditStep->setText(QString::number(n));
}

void MainWindow::setStepCount()
{
    int n=ui->lineEditStep->text().toInt();
    modelTech->setStepCount(n);
}

void MainWindow::conCheck()
{
    if (modbusDevice){
        if (modbusDevice->state()==QModbusDevice::ConnectedState){
            QModbusDataUnit readRequest(QModbusDataUnit::DiscreteInputs,1024,1);
            if (auto reply = modbusDevice->sendReadRequest(readRequest,1)){
                if (!reply->isFinished()) {
                    connect(reply, &QModbusReply::finished, this, &MainWindow::conReadyRead);
                } else {
                    //modbusDevice->disconnectDevice();
                    connectDev();
                    delete reply;
                }
            }
        } else {
            showErrMessage(tr("Ошибка связи с контроллером"));
            connectDev();
        }
    } else {
        showErrMessage(tr("Ошибка связи с контроллером"));
        connectDev();
    }
}

void MainWindow::conReadyRead()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;
    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        reply->deleteLater();
        int rc=unit.valueCount();
        if (rc!=1){
            qDebug()<<"read error";
        }
    } else {
        qDebug()<<"connection error";
        //if (modbusDevice) modbusDevice->disconnectDevice();
        connectDev();
    }
}


