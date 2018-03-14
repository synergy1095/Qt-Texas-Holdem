#include "mthread.h"

mThread::mThread(int socketDescriptor, int index, QObject *parent) :
    QThread(parent)
{
    sockdes = socketDescriptor;
    slot = index;
    socket = NULL;
}
//reimplimented run function
void mThread::run()
{
    this->socket = new QTcpSocket();

    if(!this->socket->setSocketDescriptor(sockdes))//setup socket connection
    {
//        qDebug() << "in socket error";
        emit error(socket->error()); //emit error
        return;
    }
//    qDebug() << "before connects"; //connect signal/slots
    connect(socket, SIGNAL(disconnected()), this, SLOT(on_disconnected()), Qt::DirectConnection);
    connect(socket, SIGNAL(readyRead()), this, SLOT(on_readyRead()), Qt::DirectConnection);
//    qDebug() << "after connects";
    if(this->socket->waitForReadyRead(1000)) //wait for join reply
    {
        QByteArray temp = socket->readAll();
        if(!(temp == QString("join"))) //check if reply is "join"
            this->socket->abort();
    }
    else    //if no reply drop connection
        this->socket->abort();

    emit this->connection_ready(slot);

    if(socket->isOpen()) //if socket is connected keep thread alive
        this->exec();
    emit this->finished_slot(this->slot);
    disconnect_called();
    delete socket;
    this->deleteLater();
}
//on socket disconnnected
void mThread::on_disconnected()
{
    qDebug() << " disconnected.";
    QMutex lockthis;
    lockthis.lock();
    emit this->disconnected(this->slot);

    if(this->isRunning()) //if thread is still running exit
        this->quit();
    lockthis.unlock();
}
//disconnect called
void mThread::disconnect_called()
{
//    qDebug() << "disconnect called";
    if(socket->isOpen())
        this->socket->abort();
}
//on socket ready read
void mThread::on_readyRead()
{
    QMutex lockthis;
    lockthis.lock();
    emit this->readyRead(slot); //pass along signal with slot
    lockthis.unlock();
}
//on socket ready write
void mThread::on_finWrite(qint64 bytes)
{
    QMutex lockthis;
    lockthis.lock();
    emit this->finWrite(bytes, slot); //pass along signal with slot
    lockthis.unlock();
}
//write function
void mThread::on_readyWrite(QByteArray &data)
{
    QMutex lockthis;

    lockthis.lock();
    if(this->socket->isOpen()) //if connection is able to write
    {
        this->socket->waitForBytesWritten(1000);
//        qDebug() << "writing to " << this->slot << "writing: " << data[0];
        this->socket->write(data); //write data
        this->socket->flush();
        this->socket->waitForBytesWritten(1000);
    }
    lockthis.unlock();
}


