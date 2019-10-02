#include "modelalarm.h"

ModelAlarm::ModelAlarm(int slaveAdr, int reg, int n, QModbusClient *d, QObject *parent) :  QAbstractTableModel(parent)
{
    regMess = new Regs(slaveAdr,reg,n,1000,d,this);
    connect(regMess,SIGNAL(newVal(regData)),this,SLOT(refresh(regData)));
}

int ModelAlarm::rowCount(const QModelIndex &parent) const
{
    return modelData.size();
}

int ModelAlarm::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ModelAlarm::data(const QModelIndex &index, int role) const
{
    if (role==Qt::BackgroundColorRole){
        return QVariant(QColor(255,170,170));
    }
    if (role==Qt::EditRole || role==Qt::DisplayRole){
        if (index.row()<modelData.size() && index.row()>=0){
            return modelData.at(index.row());
        }
    }
    return QVariant();
}

Qt::ItemFlags ModelAlarm::flags(const QModelIndex &index) const
{
    return (Qt::ItemIsEnabled);
}

void ModelAlarm::refresh(regData d)
{
    beginResetModel();
    modelData.clear();
    quint16 er0=d.at(0);
    quint16 er1=d.at(1);
    if (er0 & 1) modelData.push_back(QString::fromUtf8("Аварийный останов"));
    if (er0 & 2) modelData.push_back(QString::fromUtf8("Авария инвертора"));
    if (er0 & 4) modelData.push_back(QString::fromUtf8("Авария блока тиристоров 1"));
    if (er0 & 8) modelData.push_back(QString::fromUtf8("Авария блока тиристоров 2"));
    if (er0 & 16) modelData.push_back(QString::fromUtf8("Авария блока тиристоров 3"));
    if (er0 & 32) modelData.push_back(QString::fromUtf8("Авария блока тиристоров 4"));
    if (er0 & 64) modelData.push_back(QString::fromUtf8("Авария двигателя 1"));
    if (er0 & 128) modelData.push_back(QString::fromUtf8("Авария двигателя 2"));
    if (er0 & 256) modelData.push_back(QString::fromUtf8("Авария двигателя 3"));
    if (er0 & 512) modelData.push_back(QString::fromUtf8("Авария двигателя 4"));
    if (er0 & 1024) modelData.push_back(QString::fromUtf8("Авария двигателя 5"));
    if (er0 & 2048) modelData.push_back(QString::fromUtf8("Авария двигателя 6"));
    if (er0 & 4096) modelData.push_back(QString::fromUtf8("Авария двигателя 7"));
    if (er0 & 8192) modelData.push_back(QString::fromUtf8("Авария двигателя 8"));
    if (er0 & 16384) modelData.push_back(QString::fromUtf8("Авария двигателя 9"));
    if (er0 & 32768) modelData.push_back(QString::fromUtf8("Ошибка связи с температурным модулем"));
    if (er1 & 1) modelData.push_back(QString::fromUtf8("Ошибка измерения температуры (основной датчик зоны 1)"));
    if (er1 & 2) modelData.push_back(QString::fromUtf8("Ошибка измерения температуры (основной датчик зоны 2)"));
    if (er1 & 4) modelData.push_back(QString::fromUtf8("Ошибка измерения температуры (основной датчик зоны 3)"));
    if (er1 & 8) modelData.push_back(QString::fromUtf8("Ошибка измерения температуры (основной датчик зоны 4)"));
    if (er1 & 16) modelData.push_back(QString::fromUtf8("Ошибка измерения температуры (аварийный датчик зоны 1)"));
    if (er1 & 32) modelData.push_back(QString::fromUtf8("Ошибка измерения температуры (аварийный датчик зоны 2)"));
    if (er1 & 64) modelData.push_back(QString::fromUtf8("Ошибка измерения температуры (аварийный датчик зоны 3)"));
    if (er1 & 128) modelData.push_back(QString::fromUtf8("Ошибка измерения температуры (аварийный датчик зоны 4)"));
    if (er1 & 256) modelData.push_back(QString::fromUtf8("Перегрев зоны 1"));
    if (er1 & 512) modelData.push_back(QString::fromUtf8("Перегрев зоны 2"));
    if (er1 & 1024) modelData.push_back(QString::fromUtf8("Перегрев зоны 3"));
    if (er1 & 2048) modelData.push_back(QString::fromUtf8("Перегрев зоны 4"));
    if (er1 & 4096) modelData.push_back(QString::fromUtf8("Критический перегрев"));
    endResetModel();
}
