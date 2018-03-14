/********************************************************************************
** Form generated from reading UI file 'mserver.ui'
**
** Created: Thu Aug 25 18:03:23 2016
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MSERVER_H
#define UI_MSERVER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mServer
{
public:
    QGridLayout *gridLayout;
    QWidget *widget_4;
    QGridLayout *gridLayout_5;
    QListWidget *playerList;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QWidget *widget_2;
    QGridLayout *gridLayout_3;
    QPushButton *pbBack;
    QPushButton *pbQuit;
    QSpacerItem *verticalSpacer;
    QWidget *widget_3;
    QGridLayout *gridLayout_4;
    QPushButton *pbKick;
    QPushButton *pbStart;
    QLabel *playerName;
    QWidget *widget_5;
    QGridLayout *gridLayout_6;
    QLineEdit *leLocalIP;
    QLabel *label;

    void setupUi(QDialog *mServer)
    {
        if (mServer->objectName().isEmpty())
            mServer->setObjectName(QString::fromUtf8("mServer"));
        mServer->resize(500, 500);
        gridLayout = new QGridLayout(mServer);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        widget_4 = new QWidget(mServer);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        gridLayout_5 = new QGridLayout(widget_4);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        playerList = new QListWidget(widget_4);
        playerList->setObjectName(QString::fromUtf8("playerList"));

        gridLayout_5->addWidget(playerList, 1, 0, 1, 1);

        widget = new QWidget(widget_4);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(90, 0));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setMinimumSize(QSize(90, 80));
        widget_2->setMaximumSize(QSize(90, 80));
        gridLayout_3 = new QGridLayout(widget_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        pbBack = new QPushButton(widget_2);
        pbBack->setObjectName(QString::fromUtf8("pbBack"));
        pbBack->setMinimumSize(QSize(70, 25));
        pbBack->setMaximumSize(QSize(70, 25));

        gridLayout_3->addWidget(pbBack, 1, 0, 1, 1);

        pbQuit = new QPushButton(widget_2);
        pbQuit->setObjectName(QString::fromUtf8("pbQuit"));
        pbQuit->setMinimumSize(QSize(70, 25));
        pbQuit->setMaximumSize(QSize(70, 25));

        gridLayout_3->addWidget(pbQuit, 2, 0, 1, 1);


        gridLayout_2->addWidget(widget_2, 2, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 1, 0, 1, 1);

        widget_3 = new QWidget(widget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setMinimumSize(QSize(90, 80));
        widget_3->setMaximumSize(QSize(90, 80));
        gridLayout_4 = new QGridLayout(widget_3);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        pbKick = new QPushButton(widget_3);
        pbKick->setObjectName(QString::fromUtf8("pbKick"));
        pbKick->setMinimumSize(QSize(70, 25));
        pbKick->setMaximumSize(QSize(70, 25));

        gridLayout_4->addWidget(pbKick, 0, 0, 1, 1);

        pbStart = new QPushButton(widget_3);
        pbStart->setObjectName(QString::fromUtf8("pbStart"));
        pbStart->setMinimumSize(QSize(70, 25));
        pbStart->setMaximumSize(QSize(70, 25));

        gridLayout_4->addWidget(pbStart, 1, 0, 1, 1);


        gridLayout_2->addWidget(widget_3, 0, 0, 1, 1);


        gridLayout_5->addWidget(widget, 1, 1, 1, 1);

        playerName = new QLabel(widget_4);
        playerName->setObjectName(QString::fromUtf8("playerName"));

        gridLayout_5->addWidget(playerName, 0, 0, 1, 1);


        gridLayout->addWidget(widget_4, 1, 0, 1, 1);

        widget_5 = new QWidget(mServer);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        gridLayout_6 = new QGridLayout(widget_5);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        leLocalIP = new QLineEdit(widget_5);
        leLocalIP->setObjectName(QString::fromUtf8("leLocalIP"));
        leLocalIP->setMinimumSize(QSize(0, 25));
        leLocalIP->setMaximumSize(QSize(16777215, 25));

        gridLayout_6->addWidget(leLocalIP, 0, 1, 1, 1);

        label = new QLabel(widget_5);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_6->addWidget(label, 0, 0, 1, 1);


        gridLayout->addWidget(widget_5, 0, 0, 1, 1);


        retranslateUi(mServer);
        QObject::connect(pbQuit, SIGNAL(clicked()), mServer, SLOT(close()));

        QMetaObject::connectSlotsByName(mServer);
    } // setupUi

    void retranslateUi(QDialog *mServer)
    {
        mServer->setWindowTitle(QApplication::translate("mServer", "Server Lobby", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        playerList->setToolTip(QApplication::translate("mServer", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Tahoma'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">List of Players in Lobby</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pbBack->setText(QApplication::translate("mServer", "Back", 0, QApplication::UnicodeUTF8));
        pbQuit->setText(QApplication::translate("mServer", "Quit", 0, QApplication::UnicodeUTF8));
        pbKick->setText(QApplication::translate("mServer", "Kick", 0, QApplication::UnicodeUTF8));
        pbStart->setText(QApplication::translate("mServer", "Start", 0, QApplication::UnicodeUTF8));
        playerName->setText(QString());
        label->setText(QApplication::translate("mServer", "Local Host Name", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class mServer: public Ui_mServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MSERVER_H
