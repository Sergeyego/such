#ifndef MODELTECH_H
#define MODELTECH_H

#include <QObject>
#include <QAbstractTableModel>
#include <QModbusClient>
#include <QVector>
#include "regs.h"


class ModelTech : public QAbstractTableModel
{
    Q_OBJECT
public:
    ModelTech(int adr, int r, QModbusClient *d, QObject *parent = 0);
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    void setStepCount(int val);

private:
    int reg;
    QModbusClient *device;
    int slave;
    QVector<QVector<double>> dat;
    Regs *regD;
    QStringList hHeader;
private slots:
    void refresh(regData v);
signals:
    void newStepCount(int c);
};

#endif // MODELTECH_H
