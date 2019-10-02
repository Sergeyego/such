#include "modeltech.h"

ModelTech::ModelTech(int adr, int r, QModbusClient *d, QObject *parent) : slave(adr), reg(r), device(d), QAbstractTableModel(parent)
{
    hHeader.push_back(QString::fromUtf8("Время, мин"));
    hHeader.push_back(QString::fromUtf8("Темп., C"));
    regD = new Regs(adr,r,21,1000,d,this);
    connect(regD,SIGNAL(newVal(regData)),this,SLOT(refresh(regData)));
}

int ModelTech::rowCount(const QModelIndex &parent) const
{
    return dat.count();
}

int ModelTech::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant ModelTech::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
        return QVariant();
    }
    if (role==Qt::EditRole){
        return dat[index.row()][index.column()];
    }
    if (role==Qt::DisplayRole){
        int decimal = index.column()==0 ? 0 : 1;
        return QLocale().toString(dat[index.row()][index.column()],'f',decimal);
    }
    if (role==Qt::TextAlignmentRole){
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    return QVariant();
}

Qt::ItemFlags ModelTech::flags(const QModelIndex &index) const
{
    return (Qt::ItemIsEditable | Qt::ItemIsEnabled);
}

QVariant ModelTech::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role==Qt::DisplayRole && orientation==Qt::Horizontal && section<columnCount() && section>=0){
        QVariant val=hHeader.at(section);
        return val.isNull() ? QAbstractTableModel::headerData(section,orientation,role) : val;
    }
    return QAbstractTableModel::headerData(section,orientation,role);
}

bool ModelTech::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()){
        return false;
    }
    if (role==Qt::EditRole){
        dat[index.row()][index.column()]=value.toDouble();
        qint16 val= (index.column()==0) ? value.toInt() : (value.toInt()*10);
        regData d;
        d.push_back(val);
        int shift=(index.row()*2)+(index.column()+1);
        regD->writeRegs(shift,d);
    }
    return true;
}

void ModelTech::setStepCount(int val)
{
    if (val<1) val=1;
    if (val>10) val=10;
    regData d;
    d.push_back(val);
    regD->writeRegs(d);
    regD->sendReadRequest();
}

void ModelTech::refresh(regData v)
{
    int n=(qint16)v.at(0);
    if (n!=dat.size()){
        emit newStepCount(n);
    }
    if (n>dat.size()){
        beginInsertRows(QModelIndex(),dat.size(),n-1);
        dat.resize(n);
        for (int i=0; i<dat.size(); i++){
            dat[i].resize(2);
        }
        endInsertRows();
    }
    if (n<dat.size()){
        beginRemoveRows(QModelIndex(),n,(dat.size()-1));
        dat.resize(n);
        endRemoveRows();
    }
    int j=1;
    for (int i=0; i<dat.size(); i++){
        dat[i][0]=((qint16)v.at(j));
        dat[i][1]=(((qint16)v.at(j+1))/10.0);
        j+=2;
    }
    emit dataChanged(this->index(0,0),this->index(this->rowCount()-1,this->columnCount()-1));
}
