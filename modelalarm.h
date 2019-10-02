#ifndef MODELALARM_H
#define MODELALARM_H

#include <QObject>
#include <QAbstractTableModel>
#include <QModbusClient>
#include <QColor>
#include "regs.h"

class ModelAlarm : public QAbstractTableModel
{
    Q_OBJECT
public:
    ModelAlarm(int slaveAdr, int reg, int n, QModbusClient *d, QObject *parent =0);
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    QStringList modelData;
    Regs *regMess;

private slots:
    void refresh(regData d);
};

#endif // MODELALARM_H
