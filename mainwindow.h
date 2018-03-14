#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include "mclient.h"
#include "mserver.h"
#include <QString>

namespace Ui {
    class MainWindow;
}
//constructor window for program
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_rServer_toggled(bool checked);
    void on_sbPlayers_valueChanged(int arg1);
    void on_sbVictory_valueChanged(int arg1);
    void on_sbCredit_valueChanged(int arg1);
    void on_sbBlind_valueChanged(int arg1);
    void on_rClient_toggled(bool checked);
    void on_pbContinue_clicked();
    void on_back_clicked();
    void on_leName_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    mClient m_client;
    mServer m_server;
    QString pName;
    int numPlay, numVic, bBlind, sCred;
    bool isClient;
};

#endif // MAINWINDOW_H
