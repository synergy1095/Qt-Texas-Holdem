/********************************************************************************
** Form generated from reading UI file 'gamewindow.ui'
**
** Created: Thu Aug 25 18:03:23 2016
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEWINDOW_H
#define UI_GAMEWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_gameWindow
{
public:
    QGridLayout *gridLayout;
    QListWidget *status_view;
    QListView *cardView;
    QPushButton *pbQuit;
    QWidget *widget;
    QPushButton *pbFold;
    QPushButton *pbCall;
    QWidget *widget_2;
    QFormLayout *formLayout;
    QPushButton *pbRaise;
    QSpinBox *sbCred;
    QLabel *playerName;

    void setupUi(QDialog *gameWindow)
    {
        if (gameWindow->objectName().isEmpty())
            gameWindow->setObjectName(QString::fromUtf8("gameWindow"));
        gameWindow->resize(1060, 740);
        gridLayout = new QGridLayout(gameWindow);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        status_view = new QListWidget(gameWindow);
        status_view->setObjectName(QString::fromUtf8("status_view"));
        status_view->setMinimumSize(QSize(0, 600));
        status_view->setMaximumSize(QSize(16777215, 600));

        gridLayout->addWidget(status_view, 1, 0, 1, 1);

        cardView = new QListView(gameWindow);
        cardView->setObjectName(QString::fromUtf8("cardView"));
        cardView->setMinimumSize(QSize(800, 600));
        cardView->setMaximumSize(QSize(800, 600));

        gridLayout->addWidget(cardView, 1, 2, 1, 1);

        pbQuit = new QPushButton(gameWindow);
        pbQuit->setObjectName(QString::fromUtf8("pbQuit"));
        pbQuit->setMinimumSize(QSize(80, 30));
        pbQuit->setMaximumSize(QSize(80, 30));

        gridLayout->addWidget(pbQuit, 6, 0, 1, 1);

        widget = new QWidget(gameWindow);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(0, 80));
        widget->setMaximumSize(QSize(16777215, 80));
        pbFold = new QPushButton(widget);
        pbFold->setObjectName(QString::fromUtf8("pbFold"));
        pbFold->setGeometry(QRect(419, 25, 80, 30));
        pbFold->setMinimumSize(QSize(80, 30));
        pbFold->setMaximumSize(QSize(80, 30));
        pbCall = new QPushButton(widget);
        pbCall->setObjectName(QString::fromUtf8("pbCall"));
        pbCall->setGeometry(QRect(607, 25, 80, 30));
        pbCall->setMinimumSize(QSize(80, 30));
        pbCall->setMaximumSize(QSize(80, 30));
        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(111, 16, 200, 48));
        widget_2->setMinimumSize(QSize(200, 0));
        widget_2->setMaximumSize(QSize(200, 200));
        formLayout = new QFormLayout(widget_2);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        pbRaise = new QPushButton(widget_2);
        pbRaise->setObjectName(QString::fromUtf8("pbRaise"));
        pbRaise->setMinimumSize(QSize(80, 30));
        pbRaise->setMaximumSize(QSize(80, 30));

        formLayout->setWidget(0, QFormLayout::LabelRole, pbRaise);

        sbCred = new QSpinBox(widget_2);
        sbCred->setObjectName(QString::fromUtf8("sbCred"));
        sbCred->setMinimumSize(QSize(100, 30));
        sbCred->setMaximumSize(QSize(100, 30));

        formLayout->setWidget(0, QFormLayout::FieldRole, sbCred);


        gridLayout->addWidget(widget, 6, 2, 1, 1);

        playerName = new QLabel(gameWindow);
        playerName->setObjectName(QString::fromUtf8("playerName"));
        playerName->setMinimumSize(QSize(300, 30));

        gridLayout->addWidget(playerName, 0, 2, 1, 1);


        retranslateUi(gameWindow);

        QMetaObject::connectSlotsByName(gameWindow);
    } // setupUi

    void retranslateUi(QDialog *gameWindow)
    {
        gameWindow->setWindowTitle(QApplication::translate("gameWindow", "Dialog", 0, QApplication::UnicodeUTF8));
        pbQuit->setText(QApplication::translate("gameWindow", "Quit", 0, QApplication::UnicodeUTF8));
        pbFold->setText(QApplication::translate("gameWindow", "Fold", 0, QApplication::UnicodeUTF8));
        pbCall->setText(QApplication::translate("gameWindow", "Call", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pbRaise->setToolTip(QApplication::translate("gameWindow", "Raise Amount is what you currently have in the pot and credits from your credit pool combined.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pbRaise->setText(QApplication::translate("gameWindow", "Raise", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        sbCred->setToolTip(QApplication::translate("gameWindow", "Raise Amount is what you currently have in the pot and credits from your credit pool combined.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        playerName->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class gameWindow: public Ui_gameWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEWINDOW_H
