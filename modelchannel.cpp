#include "modelchannel.h"

ModelChannel::ModelChannel(int regV1, int regV2, int regUst, int regOut, QModbusClient *d, QObject *parent) :
        QAbstractTableModel(parent)
{
    curves.push_back(new QwtPlotCurve(tr("Темпер., C")));
    curves.push_back(new QwtPlotCurve(tr("Уставка, C")));
    curves.push_back(new QwtPlotCurve(tr("Выход, %")));
    curves.push_back(new QwtPlotCurve(tr("Тем. Ав, C")));
    val1 = new Regs(1,regV1,1,3000,d,this);
    ust = new Regs(1,regUst,1,3000,d,this);
    out = new Regs(1,regOut,1,3000,d,this);
    val2 = new Regs(1,regV2,1,3000,d,this);

    regMap.insert(val1,0);
    regMap.insert(ust,1);
    regMap.insert(out,2);
    regMap.insert(val2,3);

    readOnly=false;

    qwtdata.resize(curves.size());
    currentVal.resize(curves.size());
    for (int i=0; i<curves.size(); i++){
        QPen pen=curves[i]->pen();
        pen.setWidth(2);
        curves[i]->setPen(pen);
        curves[i]->attach(Plot::instance());
    }
    curves[2]->setVisible(false);
    curves[3]->setVisible(false);

    connect(val1,SIGNAL(newVal(regData)),this,SLOT(newV(regData)));
    connect(ust,SIGNAL(newVal(regData)),this,SLOT(newV(regData)));
    connect(out,SIGNAL(newVal(regData)),this,SLOT(newV(regData)));
    connect(val2,SIGNAL(newVal(regData)),this,SLOT(newV(regData)));
}

ModelChannel::~ModelChannel()
{
}

int ModelChannel::rowCount(const QModelIndex &parent) const
{
    return curves.size();
}

int ModelChannel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant ModelChannel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row()>=curves.size() || index.column()>=3) return QVariant();
    QVariant value;
    switch (role) {
        case Qt::DisplayRole:
            if (index.column()==0){
                value=curves.at(index.row())->title().text();
            } else if (index.column()==2){
                value=QLocale().toString(currentVal.at(index.row()),'f',1);
            }
        break;
        case Qt::EditRole:
            if (index.column()==1){
                value=curves.at(index.row())->pen().color();
            } else if (index.column()==2){
                value=currentVal.at(index.row());
            }
        break;
        case Qt::TextAlignmentRole:
            if (index.column()==2){
                value=int(Qt::AlignRight | Qt::AlignVCenter);
            } else {
                value=int(Qt::AlignLeft | Qt::AlignVCenter);
            }
        break;
        case Qt::CheckStateRole:
            if (index.column()==0){
                value= curves.at(index.row())->isVisible() ? Qt::Checked : Qt::Unchecked;
            }
        break;
        case Qt::BackgroundColorRole:
            if (index.column()==1){
                value=curves.at(index.row())->pen().color();
            }
        break;
        default:
            value=QVariant();
        break;
    }
    return value;
}

Qt::ItemFlags ModelChannel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags;
    int col=index.column();
    switch (col) {
        case 0:
            flags=Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
        break;
        case 1:
            flags = Qt::ItemIsEditable | Qt::ItemIsEnabled;
        break;
        case 2:
            flags= (index.row()==1 && !readOnly) ? (Qt::ItemIsEditable | Qt::ItemIsEnabled) : (Qt::ItemIsEnabled);
        break;
        default:
            flags=Qt::ItemIsEnabled;
        break;
    }
    return flags;
}

bool ModelChannel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row()>=curves.size() || index.column()>=3) return false;
    bool ok=false;
    if (role==Qt::EditRole){
        if (index.column()==1){
            QPen pen=curves[index.row()]->pen();
            pen.setColor(QColor(value.toString()));
            curves[index.row()]->setPen(pen);
            curves[index.row()]->plot()->replot();
            ok=true;
        } else if (index.column()==2){
            if (index.row()==1){
                double val=value.toDouble();
                currentVal[index.row()]=val;
                regData u;
                u.push_back(qint16(val*10.0));
                ust->writeRegs(u);
                ok=true;
                emit ustChanged(value.toDouble());
            }
        }
    }
    if (role==Qt::CheckStateRole){
        if (index.column()==0){
            curves[index.row()]->setVisible(value.toBool());
            curves[index.row()]->plot()->replot();
            ok=true;
        }
    }
    if (ok) emit dataChanged(index,index);
    return ok;
}

void ModelChannel::reloadData()
{
    for (int i=0; i<curves.size(); i++){
        curves[i]->setSamples(qwtdata.at(i).x,qwtdata.at(i).y);
    }
}

void ModelChannel::newV(regData v)
{
    auto reg = qobject_cast<Regs *>(sender());
    if (reg){
        int n=regMap.value(reg,-1);
        if (n>=0 && n< this->rowCount()){
            currentVal[n]=((int16_t)v.value(0))/10.0;
            if (n==2){
                regOut.push_back(currentVal.at(2));
            }
            emit dataChanged(this->index(n,2),this->index(n,2));
        }
    }
}

void ModelChannel::clear()
{
    for (int i=0; i<qwtdata.size(); i++){
        qwtdata[i].x.clear();
        qwtdata[i].y.clear();
    }
    reloadData();
}

void ModelChannel::plot(double t, bool truncate)
{
    if (truncate){
        for (int i=0; i<qwtdata.size(); i++){
            if (qwtdata[i].x.size()) qwtdata[i].x.removeFirst();
            if (qwtdata[i].y.size()) qwtdata[i].y.removeFirst();
        }
    }
    for (int i=0; i<qwtdata.size(); i++){
        if (i!=2){
            qwtdata[i].x.push_back(t);
            qwtdata[i].y.push_back(currentVal.at(i));
        }
    }
    double yout=0.0;
    for (int i=0; i<regOut.size(); i++){
        yout+=regOut.at(i);
    }
    if (regOut.size()) {
        yout/=regOut.size();
    }
    regOut.clear();
    qwtdata[2].x.push_back(t);
    qwtdata[2].y.push_back(yout);
    reloadData();
}


void ModelChannel::setReadOnly(bool b)
{
    readOnly=b;
}
