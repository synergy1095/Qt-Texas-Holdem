#include "qlabelclick.h"

QLabelClick::QLabelClick(QWidget *parent) :
    QLabel(parent)
{
}
//reimplimented mouse event function
void QLabelClick::mousePressEvent(QMouseEvent* ev)
{
    if(ev->button() == Qt::LeftButton)
        emit this->clicked();
}
