#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModbusDataUnit>
#include <QModbusTcpClient>
#include <QDebug>
#include "cfgplot.h"
#include "modelchannel.h"
#include "delegate.h"
#include <QColor>
#include <QTimer>
#include "mbbtn.h"
#include "mblbl.h"
#include "regs.h"
#include "modelalarm.h"
#include <QMessageBox>
#include "modeltech.h"
#include <QSettings>
#include <QMutex>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int interval, QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QMessageBox* errMessageBox;
    QModbusTcpClient *modbusDevice;
    QModbusTcpClient *mD1;
    QModbusTcpClient *mD2;
    CfgPlot *cfgPlot;
    ModelChannel *modelZone1;
    ModelChannel *modelZone2;
    ModelChannel *modelZone3;
    ModelChannel *modelZone4;
    ModelTech *modelTech;
    ModelAlarm *modelAlarm;
    QTimer *timerPlot;
    int countPlot;
    const int nMax=360;
    MbBtn *mbStart;
    MbBtn *mbStop;
    MbBtn *mbReset;
    MbBtn *mbTechStart;
    MbBtn *mbTechStop;
    MbLbl *mbLblRun;
    MbLbl *mbLblReady;
    MbLbl *mbLblAlarm;
    MbLbl *mbLblM1;
    MbLbl *mbLblM2;
    MbLbl *mbLblM3;
    MbLbl *mbLblM4;
    MbLbl *mbLblM5;
    MbLbl *mbLblM6;
    MbLbl *mbLblM7;
    MbLbl *mbLblM8;
    MbLbl *mbLblM9;
    MbLbl *mbLblTech;
    Regs *regsCount;
    Regs *regsTechStatus;
    void loadSettings();
    void saveSettings();
    QTimer *timerCon;
    //QMutex mutex;

private slots:
    void replot();
    void connectStateChanged(QModbusDevice::State state);
    void connectDev();
    void showErrMessage(QString mess);
    void newCounter(regData d);
    void newTechStatus(regData d);
    void exitPrg();
    void setAllUst();
    void newStepCount(int n);
    void setStepCount();
    void conCheck();
    void conReadyRead();

};

#endif // MAINWINDOW_H
