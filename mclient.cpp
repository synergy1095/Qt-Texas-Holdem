#include "mclient.h"
#include "ui_mclient.h"

mClient::mClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mClient)
{
    ui->setupUi(this);
    connect(&gameui, SIGNAL(quit_clicked()), this, SLOT(on_pbBack_clicked()));

    inst[0] = &mClient::send_name;
    inst[1] = &mClient::update_lobby;
    inst[2] = &mClient::name_in_use;
    inst[3] = &mClient::player_kicked;
    inst[4] = &mClient::start_game;
    inst[5] = &mClient::p_start_turn;
    inst[6] = &mClient::receive_community;
    inst[7] = &mClient::receive_pocket;
    inst[8] = &mClient::reset_screen;
    inst[9] = &mClient::update_cred;
    inst[10] = &mClient::game_ended;

    connect(&gameui, SIGNAL(p_fold()), this, SLOT(p_fold()));
    connect(&gameui, SIGNAL(p_check()), this, SLOT(p_check()));
    connect(&gameui, SIGNAL(p_call()), this, SLOT(p_call()));
    connect(&gameui, SIGNAL(p_raise(int)), this, SLOT(p_raise(int)));

    socket = NULL;
    gameui.setWindowTitle("Texas Holdem Client");
    gameStarted = false;
}

mClient::~mClient()
{
    delete ui;
}
//line edit slot holds ip address
void mClient::on_lineEdit_textChanged(const QString &arg1)
{
    host_ip = arg1;
}
 //on push button connect clicked
void mClient::on_pbCon_clicked()
{
    if(socket)
        return;
//    qDebug() << "inside connect clicked";
    gameStarted = false;
    socket = new QTcpSocket(this);
    //connect socket signals
    connect(socket,SIGNAL(connected()), this, SLOT(on_connected()));
    connect(socket,SIGNAL(disconnected()), this, SLOT(on_disconnected()));
    connect(socket,SIGNAL(readyRead()), this, SLOT(on_readyRead()));
    //connect(socket,SIGNAL(bytesWritten(qint64)),this,SLOT(on_bytesWritten(qint64)));
//    qDebug() << "before client connect";
    socket->connectToHost(host_ip,4251);
//    qDebug() << "after client connect";
    if(!socket->waitForConnected(5000)) //if failed to connect after 5 seconds
    {
        QMessageBox::information(this, "Error", socket->errorString());

        socket->deleteLater();
        socket = NULL;
    }
}
//on back button clicked
void mClient::on_pbBack_clicked()
{
    this->close();
    if(socket) //if socket exists try disconnecting then clean lobby
    {
        socket->disconnectFromHost();
        clean_up();
    }
    gameStarted = false;
    emit back_clicked();
}
//sends name to server
void mClient::send_name()
{
//    qDebug() << pName << " sent";
    socket->write(this->pName.toAscii()); //write name to server
    socket->flush();
    socket->waitForBytesWritten(100);
}
//updates lobby screen
void mClient::update_lobby()
{
    QMutex lockthis;

    lockthis.lock();
    if(gameui.isVisible())
    {
        mVector<QString> players;
        QByteArray temp;
        int cur_player;

        temp = socket->readLine();
        while(!(temp.at(temp.size()-1) - '\n'))
            temp.chop(1);
        cur_player = temp.toInt();

        while(!(socket->atEnd())) //read til end
        {
            temp = socket->readLine();
            while(!(temp.at(temp.size()-1) - '\n'))
                temp.chop(1);

            if(!(temp.size()-1)) //ensure that other junk will not show
                break;
            players.push_back(QString(temp.data())); //push back for display
        }
        socket->readAll();
        gameui.p_status->clear();
        for(int i = 0; i < players.size(); i++) //display pushed values
            gameui.p_status->addItem(players[i]);
        gameui.p_status->setCurrentRow(cur_player, QItemSelectionModel::ClearAndSelect);
    }
    else if(this->isVisible())
    {
        mVector<QString> players;
        QByteArray temp;

        while(!(socket->atEnd())) //read til end
        {
            temp = socket->readLine();
            while(!(temp.at(temp.size()-1) - '\n'))
                temp.chop(1);

            if(!(temp.size()-1)) //ensure that other junk will not show
                break;
            players.push_back(QString(temp.data())); //push back for display
        }
        socket->readAll();
        ui->playerList->clear();
        for(int i = 0; i < players.size(); i++) //display pushed values
            ui->playerList->insertItem(i, players[i]);
    }
    lockthis.unlock();
}
//name in use popup
void mClient::name_in_use()
{
    QMessageBox::information(this, "Error", "Name already taken.");
}
//kicked popup
void mClient::player_kicked()
{
    QMessageBox::information(this, "Notice", "You were kicked.");
}
//start game sent from server
void mClient::start_game()
{
    gameStarted = true;
    socket->readAll();
    gameui.player_name->setText(pName);
    gameui.player_name->setFont(QFont("Ariel", 14));
    gameui.setup_show();
    this->hide();
}
//clean up lobby
void mClient::clean_up()
{
    if(socket) //if exists tell to delete later and set to null
    {
        socket->deleteLater();
        socket = NULL;
    }
    ui->playerName->clear(); //clear ui elements
    ui->playerList->clear();
}
//player clicked fold
void mClient::p_fold()
{
    socket->write("0\n");
    socket->flush();
    socket->waitForBytesWritten(500);
}
//player clicked check
void mClient::p_check()
{
    socket->write("1\n");
    socket->flush();
    socket->waitForBytesWritten(500);
}
//player clicked call
void mClient::p_call()
{
    socket->write("2\n");
    socket->flush();
    socket->waitForBytesWritten(500);
}
//player clicked raise
void mClient::p_raise(int amount)
{
    QByteArray temp; //amount is the current high raise + the raised amount
    QString temps(QString::number(amount) + "\n");

    temp = "3\n";
    temp.append(temps);

    socket->write(temp);
    socket->flush();
    socket->waitForBytesWritten(500);
}
//server signalled player to start their turn
void mClient::p_start_turn() //inst 5
{
//    qDebug() << "in start turn client";
    QByteArray temp1 = socket->readLine();
    QByteArray temp2;
    //read current credit amount
    temp2 = socket->readLine();
    while(!(temp2[temp2.size()-1] - '\n'))
        temp2.chop(1);

    bool ok;
    gameui.cur_cred = temp2.toInt(&ok, 10);

    if(!ok)
        qDebug() << "conversion1 not successful";
    //read current amount player has in pot
    temp2 = socket->readLine();
    while(!(temp2[temp2.size()-1] - '\n'))
        temp2.chop(1);

    gameui.cur_pot_cred = temp2.toInt(&ok, 10);

    if(!ok)
        qDebug() << "conversion2 not successful";
    //read current highest raise player has to match
    temp2 = socket->readLine();
    while(!(temp2[temp2.size()-1] - '\n'))
        temp2.chop(1);

    gameui.cur_high_raise = temp2.toInt(&ok, 10);

    if(!ok)
        qDebug() << "conversion3 not successful";
    //read current pot
    temp2 = socket->readLine();
    while(!(temp2[temp2.size()-1] - '\n'))
        temp2.chop(1);

    gameui.cur_pot = temp2.toInt(&ok, 10);

    if(!ok)
        qDebug() << "conversion4 not successful";

    socket->readAll();//empty stream

    if(!(temp1[0] - '0')) //show call, fold and raise
    {
        gameui.on_turn_start_call();
    }
    else if(!(temp1[0] - '1')) //show call and fold
    {
        gameui.on_turn_start_allin();
    }
    else if(!(temp1[0] - '2')) //show check, fold, and raise
    {
        gameui.on_turn_start_check();
    }
}
//receive community card
void mClient::receive_community()
{
    QByteArray temp = socket->readLine();

    if(!(temp[0] - '0')) //set flop cards
    {
        receive_flop();
    }
    else if(!(temp[0] - '1')) //set turn card
    {
        receive_turn();
    }
    else if(!(temp[0] - '2')) // set river card
    {
        receive_river();
    }

    socket->readAll();
}
//receive flop redirected from receive_community
void mClient::receive_flop()
{
    QByteArray temp;
    for(int i = 0; i < 3; i++) //set flop cards
    {
        temp = socket->readLine();
        while(!(temp[temp.size()-1] - '\n'))
            temp.chop(1);
        QString temps = ":/images/";
        temps.append(temp.data());
        temps.append(".gif");
        QPixmap temp2(temps);
        gameui.river[i]->setPixmap(temp2);
    }
    socket->readAll();
}
//receive turn card redirected from receive_community
void mClient::receive_turn()
{
    QByteArray temp;
    temp = socket->readLine();
    while(!(temp[temp.size()-1] - '\n'))
        temp.chop(1);
    QString temps = ":/images/";
    temps.append(temp.data());
    temps.append(".gif");
    QPixmap temp2(temps);
    gameui.river[3]->setPixmap(temp2);
    socket->readAll();
}
//receive river card redirected from receive_community
void mClient::receive_river()
{
    QByteArray temp;
    temp = socket->readLine();
    while(!(temp[temp.size()-1] - '\n'))
        temp.chop(1);
    QString temps = ":/images/";
    temps.append(temp.data());
    temps.append(".gif");
    QPixmap temp2(temps);
    gameui.river[4]->setPixmap(temp2);
    socket->readAll();
}
//receive pocket cards
void mClient::receive_pocket()
{
    QByteArray temp;
    gameui.clear_pocket();
    for(int i = 0; i < 2; i++) //set pocket cards to received
    {
        temp = socket->readLine();
        while(!(temp[temp.size()-1] - '\n'))
            temp.chop(1);
        QString temps = ":/images/";
        temps.append(temp.data());
        temps.append(".gif");
        gameui.playerhand_front[i] = new QPixmap(temps);
    }
    socket->readAll();
    gameui.draw_cback();
}
// 8 reset gui
void mClient::reset_screen()
{
    for(int i = 0; i < gameui.river.size(); i++) //set flop cards
    {
        gameui.river[i]->setPixmap(*(gameui.cback));
    }
    socket->readAll();
}
// 9 update cred labels
void mClient::update_cred()
{
    QMutex lockthis;
    lockthis.lock();
    QByteArray temp2;
    //read current credit amount
    temp2 = socket->readLine();
    while(!(temp2[temp2.size()-1] - '\n'))
        temp2.chop(1);

    bool ok;
    gameui.cur_cred = temp2.toInt(&ok, 10);

    if(!ok)
        qDebug() << "conversion1 not successful";
    //read current amount player has in pot
    temp2 = socket->readLine();
    while(!(temp2[temp2.size()-1] - '\n'))
        temp2.chop(1);

    gameui.cur_pot_cred = temp2.toInt(&ok, 10);

    if(!ok)
        qDebug() << "conversion2 not successful";
    //read current highest raise player has to match
    temp2 = socket->readLine();
    while(!(temp2[temp2.size()-1] - '\n'))
        temp2.chop(1);

    gameui.cur_high_raise = temp2.toInt(&ok, 10);

    if(!ok)
        qDebug() << "conversion3 not successful";
    //read current pot
    temp2 = socket->readLine();
    while(!(temp2[temp2.size()-1] - '\n'))
        temp2.chop(1);

    gameui.cur_pot = temp2.toInt(&ok, 10);

    if(!ok)
        qDebug() << "conversion4 not successful";

    socket->readAll();//empty stream
    gameui.update_cred_labels(); //update labels
    lockthis.unlock();
}
//game end msg box
void mClient::game_ended()
{
    socket->readAll();
    QMessageBox::information(this, "Game Ended", "Game has ended thanks for playing!");
}
//slot for on connect
void mClient::on_connected()
{
    if(!(socket->write("join")))
        QMessageBox::information(this, "Error", "Connection Error.");
    ui->playerName->setText("Player Name: " + pName);
}
// slot for disconnect
void mClient::on_disconnected()
{
    QMessageBox::information(this, "Disconnected", "You have disconnected.");
    clean_up();
    emit back_clicked();
    if(gameui.isVisible())
    {
        gameui.close();
    }
    close();
}
//slot triggers once socket::readyRead triggered
void mClient::on_readyRead()
{
    QMutex lockthis;

    lockthis.lock();
    QByteArray temp = socket->readLine();
//    qDebug() << "received switch " << temp[0];
    int slot = temp[0] - '0'; //get slot value

    if((slot > 10) || (slot < 0)) //if outside the slot limit return
        return;

//    qDebug() << slot << " switch";
    (this->*(inst[slot]))(); //switches to correct function
//    socket->readAll();
    lockthis.unlock();
}
//when ready button clicked
void mClient::on_pbReady_clicked()
{
    if(!socket)
        return;

    QMutex lockthis;

    lockthis.lock();
    if(socket->isOpen()) //if connection is able to write/read
    {
        socket->write("4"); //send ready toggle
        socket->flush();
        socket->waitForBytesWritten(500);
    }
    lockthis.unlock();
}
