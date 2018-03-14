/********************************************************************************
** Form generated from reading UI file 'mclient.ui'
**
** Created: Thu Aug 25 18:03:23 2016
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MCLIENT_H
#define UI_MCLIENT_H

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

class Ui_mClient
{
public:
    QGridLayout *gridLayout_2;
    QWidget *widget;
    QGridLayout *gridLayout_3;
    QLineEdit *lineEdit;
    QPushButton *pbCon;
    QLabel *label;
    QWidget *widget_2;
    QGridLayout *gridLayout;
    QListWidget *playerList;
    QWidget *widget_3;
    QGridLayout *gridLayout_4;
    QWidget *widget_5;
    QGridLayout *gridLayout_6;
    QPushButton *pbReady;
    QWidget *widget_4;
    QGridLayout *gridLayout_5;
    QPushButton *pbBack;
    QPushButton *pbQuit;
    QSpacerItem *verticalSpacer;
    QLabel *playerName;

    void setupUi(QDialog *mClient)
    {
        if (mClient->objectName().isEmpty())
            mClient->setObjectName(QString::fromUtf8("mClient"));
        mClient->resize(500, 500);
        gridLayout_2 = new QGridLayout(mClient);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        widget = new QWidget(mClient);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout_3 = new QGridLayout(widget);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(0, 25));
        lineEdit->setMaximumSize(QSize(16777215, 25));

        gridLayout_3->addWidget(lineEdit, 0, 1, 1, 1);

        pbCon = new QPushButton(widget);
        pbCon->setObjectName(QString::fromUtf8("pbCon"));
        pbCon->setMinimumSize(QSize(75, 25));
        pbCon->setMaximumSize(QSize(75, 25));

        gridLayout_3->addWidget(pbCon, 0, 2, 1, 1);

        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_3->addWidget(label, 0, 0, 1, 1);


        gridLayout_2->addWidget(widget, 1, 0, 1, 1);

        widget_2 = new QWidget(mClient);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        gridLayout = new QGridLayout(widget_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        playerList = new QListWidget(widget_2);
        playerList->setObjectName(QString::fromUtf8("playerList"));

        gridLayout->addWidget(playerList, 1, 0, 1, 1);

        widget_3 = new QWidget(widget_2);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setMinimumSize(QSize(90, 0));
        gridLayout_4 = new QGridLayout(widget_3);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        widget_5 = new QWidget(widget_3);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        gridLayout_6 = new QGridLayout(widget_5);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        pbReady = new QPushButton(widget_5);
        pbReady->setObjectName(QString::fromUtf8("pbReady"));
        pbReady->setMinimumSize(QSize(70, 25));
        pbReady->setMaximumSize(QSize(70, 25));

        gridLayout_6->addWidget(pbReady, 0, 0, 1, 1);


        gridLayout_4->addWidget(widget_5, 0, 0, 1, 1);

        widget_4 = new QWidget(widget_3);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        gridLayout_5 = new QGridLayout(widget_4);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        pbBack = new QPushButton(widget_4);
        pbBack->setObjectName(QString::fromUtf8("pbBack"));
        pbBack->setMinimumSize(QSize(70, 25));
        pbBack->setMaximumSize(QSize(70, 25));

        gridLayout_5->addWidget(pbBack, 0, 0, 1, 1);

        pbQuit = new QPushButton(widget_4);
        pbQuit->setObjectName(QString::fromUtf8("pbQuit"));
        pbQuit->setMinimumSize(QSize(70, 25));
        pbQuit->setMaximumSize(QSize(70, 25));

        gridLayout_5->addWidget(pbQuit, 1, 0, 1, 1);


        gridLayout_4->addWidget(widget_4, 2, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer, 1, 0, 1, 1);


        gridLayout->addWidget(widget_3, 1, 1, 1, 1);

        playerName = new QLabel(widget_2);
        playerName->setObjectName(QString::fromUtf8("playerName"));

        gridLayout->addWidget(playerName, 0, 0, 1, 1);


        gridLayout_2->addWidget(widget_2, 0, 0, 1, 1);


        retranslateUi(mClient);
        QObject::connect(pbQuit, SIGNAL(clicked()), mClient, SLOT(close()));
        QObject::connect(lineEdit, SIGNAL(returnPressed()), pbCon, SLOT(click()));

        QMetaObject::connectSlotsByName(mClient);
    } // setupUi

    void retranslateUi(QDialog *mClient)
    {
        mClient->setWindowTitle(QApplication::translate("mClient", "Client Lobby", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit->setToolTip(QApplication::translate("mClient", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Enter the IP address of the Server you want to connect to.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pbCon->setToolTip(QApplication::translate("mClient", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Enter the IP address of the Server you want to connect to.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pbCon->setText(QApplication::translate("mClient", "Connect", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label->setToolTip(QApplication::translate("mClient", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Enter the IP address of the Server you want to connect to.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("mClient", "Host IP", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        playerList->setToolTip(QApplication::translate("mClient", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Tahoma'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">List of Players in Lobby</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pbReady->setText(QApplication::translate("mClient", "Ready", 0, QApplication::UnicodeUTF8));
        pbBack->setText(QApplication::translate("mClient", "Back", 0, QApplication::UnicodeUTF8));
        pbQuit->setText(QApplication::translate("mClient", "Quit", 0, QApplication::UnicodeUTF8));
        playerName->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class mClient: public Ui_mClient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MCLIENT_H
