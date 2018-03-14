#ifndef QLABELCLICK_H
#define QLABELCLICK_H

#include <QLabel>
#include <QMouseEvent>

class QLabelClick : public QLabel
{
    Q_OBJECT
public:
    explicit QLabelClick(QWidget *parent = 0);
    
signals:
    void clicked(); //if this label is clicked
public slots:
    void mousePressEvent(QMouseEvent *ev); //reimplimented mouse event function
    
};

#endif // QLABELCLICK_H
