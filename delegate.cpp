#include "delegate.h"

ColorDelegate::ColorDelegate(QObject *parent):QItemDelegate(parent)
{

}

QWidget *ColorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QColorDialog *d = new QColorDialog(parent);
    return d;
}

void ColorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QColorDialog *d= static_cast<QColorDialog*>(editor);
    if (d){
        QColor color = QColor(index.model()->data(index, Qt::EditRole).toString());
        d->setCurrentColor(color);
    }
    return;
}

void ColorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QColorDialog *d= static_cast<QColorDialog*>(editor);
    if (d){
        model->setData(index,d->currentColor());
    }
}

bool ColorDelegate::eventFilter(QObject *object, QEvent *event)
{
    if (event->type()== QEvent::FocusOut)
        return false;
    return QItemDelegate::eventFilter(object,event);
}

LineDelegate::LineDelegate(QObject *parent):QItemDelegate(parent)
{

}

QWidget *LineDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    LineEdit *e= new LineEdit(parent);
    QDoubleValidator *validator = new QDoubleValidator(parent);
    validator->setLocale(QLocale::English);
    e->setValidator(validator);
    e->setText(index.model()->data(index,Qt::EditRole).toString());
    return e;
}

void LineDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    /*LineEdit *le = static_cast<LineEdit*>(editor);
    if (le){
        le->setText(index.model()->data(index,Qt::EditRole).toString());
    }*/
    return;
}

void LineDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    LineEdit *le = static_cast<LineEdit*>(editor);
    if (le){
        bool ok;
        double val=le->text().toDouble(&ok);
        if (ok)
            model->setData(index,val);
        else if (le->text().isEmpty())
            model->setData(index,QVariant());
    }
}

void LineDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void LineDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QItemDelegate::paint(painter,option,index);
}

LineColorDelegate::LineColorDelegate(QObject *parent) : LineDelegate(parent)
{

}

void LineColorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString text;
    QVariant value;
    QStyleOptionViewItemV2 opt = setOptions(index, option);
    value = index.data(Qt::EditRole);
    if (value.typeName()==QString("double")) {
        opt.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
        opt.font=QFont(opt.font.family(),16);
        if (index.row()==0) opt.palette.setColor(QPalette::Text,QColor(255,0,0));
        if (index.row()==1) opt.palette.setColor(QPalette::Text,QColor(0,85,0));
        text=QLocale().toString(value.toDouble(), 'f',1);
    } else text = value.toString();
    drawDisplay(painter, opt, opt.rect, text);
}
