/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu Aug 25 18:03:23 2016
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QWidget *widget_2;
    QGridLayout *gridLayout_2;
    QWidget *widget;
    QRadioButton *rServer;
    QSpinBox *sbVictory;
    QSpinBox *sbCredit;
    QSpinBox *sbPlayers;
    QSpinBox *sbBlind;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QRadioButton *rClient;
    QPushButton *pushButton;
    QPushButton *pbContinue;
    QWidget *widget_3;
    QFormLayout *formLayout;
    QLabel *label_7;
    QLabel *label_6;
    QLineEdit *leName;
    QSpacerItem *verticalSpacer;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(387, 347);
        MainWindow->setMinimumSize(QSize(0, 0));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setMinimumSize(QSize(0, 220));
        widget_2->setMaximumSize(QSize(16777215, 220));
        gridLayout_2 = new QGridLayout(widget_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        widget = new QWidget(widget_2);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(200, 200));
        widget->setMaximumSize(QSize(200, 200));
        rServer = new QRadioButton(widget);
        rServer->setObjectName(QString::fromUtf8("rServer"));
        rServer->setGeometry(QRect(20, 10, 82, 17));
        sbVictory = new QSpinBox(widget);
        sbVictory->setObjectName(QString::fromUtf8("sbVictory"));
        sbVictory->setGeometry(QRect(10, 70, 42, 22));
        sbVictory->setMinimum(1);
        sbVictory->setMaximum(1);
        sbCredit = new QSpinBox(widget);
        sbCredit->setObjectName(QString::fromUtf8("sbCredit"));
        sbCredit->setGeometry(QRect(10, 100, 91, 22));
        sbCredit->setMinimum(1);
        sbCredit->setMaximum(999999999);
        sbPlayers = new QSpinBox(widget);
        sbPlayers->setObjectName(QString::fromUtf8("sbPlayers"));
        sbPlayers->setGeometry(QRect(10, 40, 42, 22));
        sbPlayers->setMinimum(2);
        sbPlayers->setMaximum(9);
        sbBlind = new QSpinBox(widget);
        sbBlind->setObjectName(QString::fromUtf8("sbBlind"));
        sbBlind->setGeometry(QRect(10, 130, 91, 22));
        sbBlind->setMinimum(1);
        sbBlind->setMaximum(999999999);
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 40, 111, 16));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(70, 70, 111, 16));
        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(110, 100, 101, 16));
        label_4 = new QLabel(widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(110, 130, 101, 16));
        label_5 = new QLabel(widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(90, 10, 31, 16));
        rClient = new QRadioButton(widget);
        rClient->setObjectName(QString::fromUtf8("rClient"));
        rClient->setGeometry(QRect(130, 10, 50, 17));
        rClient->setChecked(true);
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 170, 75, 25));
        pushButton->setMinimumSize(QSize(75, 25));
        pushButton->setMaximumSize(QSize(75, 25));
        pbContinue = new QPushButton(widget);
        pbContinue->setObjectName(QString::fromUtf8("pbContinue"));
        pbContinue->setGeometry(QRect(110, 170, 75, 25));
        pbContinue->setMinimumSize(QSize(75, 25));
        pbContinue->setMaximumSize(QSize(75, 25));

        gridLayout_2->addWidget(widget, 0, 0, 1, 1);


        gridLayout->addWidget(widget_2, 2, 1, 1, 1);

        widget_3 = new QWidget(centralWidget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setMinimumSize(QSize(0, 60));
        formLayout = new QFormLayout(widget_3);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_7 = new QLabel(widget_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout->setWidget(0, QFormLayout::FieldRole, label_7);

        label_6 = new QLabel(widget_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_6);

        leName = new QLineEdit(widget_3);
        leName->setObjectName(QString::fromUtf8("leName"));
        leName->setMinimumSize(QSize(0, 25));
        leName->setMaximumSize(QSize(16777215, 25));

        formLayout->setWidget(1, QFormLayout::FieldRole, leName);


        gridLayout->addWidget(widget_3, 1, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 999999999, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 387, 19));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(pbContinue, leName);
        QWidget::setTabOrder(leName, rServer);
        QWidget::setTabOrder(rServer, rClient);
        QWidget::setTabOrder(rClient, sbPlayers);
        QWidget::setTabOrder(sbPlayers, sbVictory);
        QWidget::setTabOrder(sbVictory, sbCredit);
        QWidget::setTabOrder(sbCredit, sbBlind);
        QWidget::setTabOrder(sbBlind, pushButton);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionExit);

        retranslateUi(MainWindow);
        QObject::connect(actionExit, SIGNAL(activated()), MainWindow, SLOT(close()));
        QObject::connect(pushButton, SIGNAL(clicked()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Texas Holdem Settings", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        rServer->setText(QApplication::translate("MainWindow", "Server", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Max Players (up to 9)", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "# of Players Remaing", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Starting Credit", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Big Blind Amount", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "-or-", 0, QApplication::UnicodeUTF8));
        rClient->setText(QApplication::translate("MainWindow", "Client", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        pbContinue->setText(QApplication::translate("MainWindow", "Continue", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "(Name have a length greather than 2.)", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Create Name", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
