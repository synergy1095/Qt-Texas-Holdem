#include "mqtcpserver.h"

mQTcpServer::mQTcpServer(QObject *parent) :
    QTcpServer(parent)
{
    accept_con = true;
    cur_con = 0;   
}

mQTcpServer::~mQTcpServer()
{
    nukem();
    threads.clear();
}
//starts server
void mQTcpServer::startServer(int connection_cap)
{
    if(!accept_con) //if server is not allowing connections return
        return;

    con_cap = connection_cap; //max # of connections
    threads.resize(con_cap, NULL);

    //qDebug() << "in start server";
    if(!this->listen(QHostAddress::Any,4251)) //if listen returns false send error
    {
        emit server_start_error(QString("Error, server unable to start."));
    }
    else    //server started successfully
        qDebug() << "server started";
}
//resets server to default
void mQTcpServer::resetServer()
{
    nukem();
}
//resets server to default values
void mQTcpServer::nukem()
{
//    qDebug() << "in nukem";
    if(!(threads.size())) //if size is 0 already return
        return;
    cur_con = 0;
    for(int i = 0; i < threads.size(); i++)
    {
        if(threads[i])
        {
            if(!(threads[i]->isFinished()))
                threads[i]->quit();
        }
    }
    if(this->isListening()) //close connection if still listening
        this->close();
//    qDebug() << "end of nuke ";
}

void mQTcpServer::incomingConnection(int socketDescriptor)
{
    qDebug() << socketDescriptor << " in incoming";
    if(!(cur_con-con_cap) || !accept_con) //if at connection cap or not accepting connections
    {
        QTcpSocket slap;
        slap.setSocketDescriptor(socketDescriptor); //disconnect new connection and stop listening
        if(slap.isValid())
            slap.abort();
        qDebug() << "extra joiner slapped down";
        this->close();
        return;
    }

    mThread *thread;
    cur_con++;      //keep track of new connection count
    thread = new mThread(socketDescriptor, 0);
//    qDebug() << "before for";
    for(int i = 0; i < threads.size(); i++)
    {
        if(!(threads[i]))
        {
//            qDebug() << "before equal";
            thread->slot = i;       //store what slot thread is in
            connect(thread, SIGNAL(finished_slot(int)),this, SLOT(on_finished(int)), Qt::DirectConnection); //set connect for signal/slots
            connect(thread, SIGNAL(disconnected(int)), this, SLOT(on_disconnected(int)), Qt::BlockingQueuedConnection);
            connect(thread, SIGNAL(connection_ready(int)), this, SLOT(on_connection_ready(int)), Qt::DirectConnection);
            threads[i] = thread; //store new connection
            break;
        }
    }

    thread->start();
//    qDebug() << "end of incom";
}
//if player dced pass along slot
void mQTcpServer::on_disconnected(int slot)
{
//    qDebug() << "on disc";
    cur_con--;  //decrease current connections

    emit this->player_drop(slot);

    if(!(this->isListening()) && accept_con) //if not listening and accepting connections start listening
    {
        if(!this->listen(QHostAddress::Any,4251))
        {
            emit server_start_error(QString("Error, server unable to start."));
        }
    }
//    qDebug() << "end disc";
}
//if thread finished
void mQTcpServer::on_finished(int slot)
{
    threads[slot] = NULL; //on finished signal set emit slot null
}
//used to pass along new connection slot
void mQTcpServer::on_connection_ready(int slot)
{
    emit create_player(slot);
}
