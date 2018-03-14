#ifndef MQTCPSERVER_H
#define MQTCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include "mvector.h"
#include "mthread.h"

class mQTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit mQTcpServer(QObject *parent = 0);
    ~mQTcpServer();
    void startServer(int connection_cap); //starts server
    void resetServer(); //resets server to default
signals:
    void server_start_error(QString errorString); //signals server start error
    void create_player(int slot);   //signal used to pass slot created in
    void player_drop(int slot);     //signal used to pass which slot player dced from
private slots:
    void on_disconnected(int slot); //if player dced pass along slot
    void on_finished(int slot); //if thread finished
    void on_connection_ready(int slot); //used to pass along new connection slot

protected:
    void incomingConnection(int socketDescriptor); //reimplimented incomingConnection

private:
    void nukem(); //resets server to default values

    bool accept_con;
    int con_cap, cur_con;
    mVector<mThread*> threads;
    friend class mServer;
};

#endif // MQTCPSERVER_H
