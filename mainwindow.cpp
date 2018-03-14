#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isClient = true;

    connect(&m_server, SIGNAL(back_clicked()), this, SLOT(on_back_clicked()));
    connect(&m_client, SIGNAL(back_clicked()), this, SLOT(on_back_clicked()));

    QTime now = QTime::currentTime(); //seeder for qrand
    qsrand(now.msec());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_rServer_toggled(bool checked)
{
    isClient = !checked;
    ui->rClient->setChecked(false);
}

void MainWindow::on_sbPlayers_valueChanged(int arg1)
{
    ui->rServer->setChecked(true);
    m_server.numPlay = arg1;
    ui->sbVictory->setMaximum(m_server.numPlay-1);
}

void MainWindow::on_sbVictory_valueChanged(int arg1)
{
    ui->rServer->setChecked(true);
    m_server.numVic = arg1;
}


void MainWindow::on_sbCredit_valueChanged(int arg1)
{
    ui->rServer->setChecked(true);
    m_server.sCred = arg1;
}


void MainWindow::on_sbBlind_valueChanged(int arg1)
{
    ui->rServer->setChecked(true);
    m_server.bBlind = arg1;
}

void MainWindow::on_rClient_toggled(bool checked)
{
    //isClient = checked;
    ui->rServer->setChecked(false);
}

void MainWindow::on_pbContinue_clicked()
{
    if(pName.isEmpty())
    {
        QMessageBox::information(this, "Error", "Please enter a player name.");
        return;
    }
    if(pName.size() < 3)
    {
        QMessageBox::information(this, "Error", "Player name must be longer than 2 characters in length.");
        return;
    }

    this->hide();
    if(isClient)
    {
        m_client.pName = pName;
        m_client.setModal(true);
        m_client.exec();
    }
    else
    {
        m_server.pName = pName;
        m_server.startServer();
        m_server.setModal(true);
        m_server.exec();
    }
}

void MainWindow::on_back_clicked()
{
    this->show();
}

void MainWindow::on_leName_textChanged(const QString &arg1)
{
    pName = arg1;
}
