#ifndef MODELCHANNEL_H
#define MODELCHANNEL_H

#include <QAbstractTableModel>
#include <QColor>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_point_data.h>
#include "cfgplot.h"
#include <qwt_symbol.h>
#include "regs.h"
#include <QModbusClient>

struct pnt{
    QVector<double> x;
    QVector<double> y;
};


class ModelChannel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ModelChannel(int regV1, int regV2, int regUst, int regOut, QModbusClient *d, QObject *parent = 0);
    ~ModelChannel();
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);

private:
    QVector <QwtPlotCurve*> curves;
    QVector <pnt> qwtdata;
    QVector <double> currentVal;
    void reloadData();
    bool readOnly;
    QVector <double> regOut;
    QMap <Regs*, int > regMap;
    Regs *val1;
    Regs *ust;
    Regs *out;
    Regs *val2;

private slots:
    void newV(regData v);

signals:
    void ustChanged(double ust);

public slots:
    void clear();
    void plot(double t, bool truncate);
    void setReadOnly(bool b);
};

#endif // MODELCHANNEL_H
