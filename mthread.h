#ifndef MTHREAD_H
#define MTHREAD_H

#include <QThread>
#include <QString>
#include <QMutex>
#include <QTcpSocket>

class mThread : public QThread
{
    Q_OBJECT
public:
    explicit mThread(int socketDescriptor, int index = 0, QObject *parent = 0);
    void run(); //reimplimented run function

signals:
    void error(QTcpSocket::SocketError sError); //pass along error
    void readyRead(int slot);       //socket ready to read
    void finWrite(qint64 bytes, int slot); //finished writing
    void connection_ready(int slot); //signal new connection
    void disconnected(int slot); //signal disconnected
    void finished_slot(int slot); //signal finished
    void call_disconnect(); //signal disconnect called
private slots:
    void on_readyRead(); //on socket ready read
    void on_finWrite(qint64 bytes); //on socket ready write
    void on_disconnected(); //on socket disconnnected
    void disconnect_called(); //disconnect called

public slots:

private:
    void on_readyWrite(QByteArray &data); //write function

    QTcpSocket *socket;
    int sockdes;
    int slot;
    friend class mQTcpServer;
    friend class mServer;
};

#endif // MTHREAD_H
