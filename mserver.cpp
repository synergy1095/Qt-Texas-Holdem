#include "mserver.h"
#include "ui_mserver.h"

mServer::mServer(QWidget *parent) :
    QDialog(parent),   ui(new Ui::mServer)
{
    ui->setupUi(this);
    numPlay = 2;
    numVic = 1;
    sCred = 1;
    bBlind = 1;
    sBlind = 0;
    connect(ui->pbBack, SIGNAL(clicked()), this, SLOT(on_back_clicked()));
    connect(&gameui, SIGNAL(quit_clicked()), this, SLOT(on_back_clicked()));
    connect(&tServer, SIGNAL(server_start_error(QString)), this, SLOT(error_msg(QString)));
    connect(&tServer, SIGNAL(create_player(int)), this, SLOT(on_create_player(int)), Qt::DirectConnection);
    connect(&tServer, SIGNAL(player_drop(int)), this, SLOT(on_player_drop(int)), Qt::DirectConnection);
    connect(&gameui, SIGNAL(p_call()), this, SLOT(on_game_call_clicked()), Qt::DirectConnection);
    connect(&gameui, SIGNAL(p_check()), this, SLOT(on_game_check_clicked()), Qt::DirectConnection);
    connect(&gameui, SIGNAL(p_fold()), this, SLOT(on_game_fold_clicked()), Qt::DirectConnection);
    connect(&gameui, SIGNAL(p_raise(int)), this, SLOT(on_game_raise_clicked(int)), Qt::DirectConnection);
    ui->leLocalIP->setReadOnly(true);
    ui->leLocalIP->setText(QHostInfo::localHostName());

    inst_serv[4] = &mServer::player_toggle_ready;

    gameui.setWindowTitle("Texas Holdem Server");
}

mServer::~mServer()
{
    tServer.close();
    tServer.resetServer();
    nTree.clear();
    delete ui;
}

void mServer::startServer() //start function
{
    nameData temp;
    temp.pName = this->pName;
    temp.slot = -1;
    sBlind = bBlind/2;
    gameStarted = false;
    nTree.insert(temp);

    player_ready.resize(numPlay -1);
    player_stats.resize(numPlay);
    player_stats[0] = new Player(&this->pName, &this->river, sCred);

    for(int i = 1; i < player_stats.size(); i++)
        player_stats[i] = NULL;
    for(int i = 0; i < player_ready.size(); i++)
        player_ready[i] = '0';
    tServer.accept_con = true;
    tServer.startServer(numPlay -1);
    ui->playerList->clear();
    ui->playerName->setText("Player Name: " + pName);
    ui->playerList->insertItem(0, pName);
}


void mServer::on_back_clicked()
{
    tServer.accept_con = false;
    gameStarted = false;
    nTree.clear();
    player_ready.clear();
    tServer.resetServer();
    emit back_clicked();
    if(gameui.isVisible())
    {
        gameui.close();
    }
    this->close();
}

void mServer::error_msg(QString errorString)
{
    QMessageBox::information(this, "Error", errorString);
}
//function set's up new player on connection
void mServer::on_create_player(int slot)
{
    QMutex lockthis;

    lockthis.lock();
    //    qDebug() << "in create play";
    QByteArray cur_data = "0";
    //    qDebug() << "after data";
    tServer.threads[slot]->on_readyWrite(cur_data);
    //    qDebug() << "after name req";
    if(run_waitRead(slot, 5000))
    {
        //        qDebug() << "inside if";
        QByteArray temp = tServer.threads[slot]->socket->readLine();
        tServer.threads[slot]->socket->readAll();
        while(!(temp.at(temp.size()-1) - '\n') || !(temp.at(temp.size()-1)))
            temp.chop(1);

        nameData tempName;
        QString temps(temp.data());
        tempName.pName = temps;
        tempName.slot = slot;
        //        qDebug() << "before second if";
        if(nTree.find(tempName))
        {
            cur_data = "2";
            tServer.threads[slot]->on_readyWrite(cur_data);
            run_waitWrite(slot, 2000);
            tServer.threads[slot]->quit();
            return;
        }
        nameData* tempn;
        tempn = nTree.insert(tempName);

        player_stats[slot+1] = new Player(&tempn->pName,&river,sCred);


        connect(tServer.threads[slot], SIGNAL(readyRead(int)), this, SLOT(on_ready_read(int)));

        if(ui->playerList->item(slot+1))
            delete ui->playerList->item(slot+1);
        ui->playerList->insertItem(slot+1, temps);
        //        tServer.threads[slot]->pName = &(tempn->pName);
        player_ready[slot] = '0';
        send_lobby_update();
    }
    else
        tServer.threads[slot]->quit();

    lockthis.unlock();
    //        qDebug() << "after create play";
}
//function cleans up after player disconnects
void mServer::on_player_drop(int slot)
{
    QMutex lockthis;

    lockthis.lock();
    //    qDebug() << "in player drop0";
    if(!(player_stats[slot+1]))
        return;
    if(!nTree.count())
        return;
    //    qDebug() << "in player drop1";
    player_ready[slot] = '0';

    nameData tempName;
    tempName.pName = *(player_stats[slot+1]->pName);
    delete player_stats[slot+1];
    player_stats[slot+1] = NULL;
    tempName.slot = -1;
    nTree.remove(tempName);

    if(this->isVisible())
    {
        if(ui->playerList->item(slot+1))
            delete ui->playerList->item(slot+1);
        ui->playerList->insertItem(slot+1, " ");
        send_lobby_update();
    }
    lockthis.unlock();
}

void mServer::on_ready_read(int slot)
{
    if(gameui.isVisible())
        return;

    QByteArray temp = tServer.threads[slot]->socket->readLine();
    int i = temp[0] - '0';

    if(i <= 9)
        (this->*(inst_serv[i]))(slot);
}
//lobby update function
void mServer::send_lobby_update()
{
    QMutex lockthis;

    lockthis.lock();

    if(gameui.isVisible()) //if game started update player status instead
    {
        gameui.p_status->clear();
        for(int i = 0; i < player_stats.size(); i++)
        {
            if(player_stats[i]) //0 folded, 1 check, 2 call, 3 raise, 4 none
            {
                if(!(player_stats[i]->isActive))
                    gameui.p_status->addItem(*(player_stats[i]->pName) + " Out");
                else if(!(player_stats[i]->action))
                    gameui.p_status->addItem(*(player_stats[i]->pName) + " Fold");
                else if(!(player_stats[i]->action - 1))
                    gameui.p_status->addItem(*(player_stats[i]->pName) + " Check");
                else if(!(player_stats[i]->action - 2))
                    gameui.p_status->addItem(*(player_stats[i]->pName) + " Call");
                else if(!(player_stats[i]->action - 3))
                    gameui.p_status->addItem(*(player_stats[i]->pName) + " Raise");
                else
                    gameui.p_status->addItem(*(player_stats[i]->pName));
            }
            else
                gameui.p_status->addItem(" ");
        }
        gameui.p_status->setCurrentRow(cur_player, QItemSelectionModel::ClearAndSelect);

        QString data("1\n");
        data.append(QString::number(cur_player) + "\n");
        for(int i = 0; i < player_stats.size(); i++)
        {
            if(player_stats[i]) //0 folded, 1 check, 2 call, 3 raise, 4 none
            {
                if(!(player_stats[i]->isActive))
                    data.append(*(player_stats[i]->pName) + " Out");
                else if(!(player_stats[i]->action))
                    data.append(*(player_stats[i]->pName) + " Fold");
                else if(!(player_stats[i]->action - 1))
                    data.append(*(player_stats[i]->pName) + " Check");
                else if(!(player_stats[i]->action - 2))
                    data.append(*(player_stats[i]->pName) + " Call");
                else if(!(player_stats[i]->action - 3))
                    data.append(*(player_stats[i]->pName) + " Raise");
                else if(!(player_stats[i]->action - 5))
                    data.append(*(player_stats[i]->pName) + " Out");
                else
                    data.append(*(player_stats[i]->pName));

                data.append("\n");
            }
            else
                data.append("  \n");
        }
        data.append(" \n");
        QByteArray sendThis(data.toAscii());
        send_to_all(sendThis);
    }
    else if(this->isVisible()) //lobby update
    {
        QString data("1\n");
        data.append(pName + "\n");

        for(int i = 1; i < player_stats.size(); i++)
        {
            if(player_stats[i])
            {
                if(player_ready[i-1] - '0')
                    data.append(*(player_stats[i]->pName) + " -Ready\n");
                else
                    data.append(*(player_stats[i]->pName) + "\n");
            }
            else
                data.append("  \n");
        }
        data.append(" \n");
        QByteArray sendThis(data.toAscii());
        send_to_all(sendThis);
    }
    lockthis.unlock();
}
//update all labels
void mServer::send_cred_update()
{
    if(player_stats[0])
    {
        gameui.cur_cred = player_stats[0]->credits;
        gameui.cur_pot_cred = player_stats[0]->credInPot;
        gameui.cur_high_raise = this->highRaise;
        gameui.cur_pot = this->pot;
        gameui.update_cred_labels();
    }
    for(int i = 0; i < tServer.threads.size(); i++)
    {
        if(tServer.threads[i])
        {
            QByteArray temp = "9\n";
            temp.append(QString::number(player_stats[i+1]->credits) + "\n");
            temp.append(QString::number(player_stats[i+1]->credInPot) + "\n");
            temp.append(QString::number(highRaise) + "\n");
            temp.append(QString::number(pot) + "\n");
            tServer.threads[i]->on_readyWrite(temp);
        }
    }
    run_waitGame(100);
}
//send data to all connections
void mServer::send_to_all(QByteArray &data)
{
    QMutex lockthis;

    lockthis.lock();
    for(int i = 0; i < tServer.threads.size(); i++)
    {
        if(tServer.threads[i])
        {
            if(tServer.threads[i]->isRunning())
            {
                tServer.threads[i]->on_readyWrite(data);
            }
        }
    }
    run_waitGame(100);
    lockthis.unlock();
}

bool mServer::run_waitRead(int slot, int mstime)
{
    if(!tServer.threads[slot])
        return false;
    if(!tServer.threads[slot]->socket->isOpen())
        return false;

    //    qDebug() << "before read connects";
    QTimer ticktock;
    QEventLoop waitRead;
    ticktock.setInterval(mstime);
    ticktock.setSingleShot(false);
    connect((tServer.threads[slot]->socket), SIGNAL(readyRead()), &waitRead, SLOT(quit()), Qt::DirectConnection);
    connect(&ticktock, SIGNAL(timeout()), &waitRead, SLOT(quit()), Qt::DirectConnection);
    connect((tServer.threads[slot]->socket), SIGNAL(disconnected()),&waitRead, SLOT(quit()), Qt::DirectConnection);
    connect(&gameui, SIGNAL(quit_clicked()), &waitRead, SLOT(quit()), Qt::DirectConnection);
    //    qDebug() << "after read connects";

    if(tServer.threads[slot]->socket->bytesAvailable())
        return true;

    ticktock.start();
    waitRead.exec();

    //    qDebug() << tServer.threads[slot]->socket->bytesAvailable() << " bytes available to read";

    if (!tServer.threads[slot] || (gameStarted && !gameui.isVisible()))
        return false;
    if(tServer.threads[slot]->socket->bytesAvailable())
        return true;
    else
        return false;
}

bool mServer::run_waitWrite(int slot, int mstime)
{
    if(!tServer.threads[slot])
        return false;
    if(!tServer.threads[slot]->socket->isOpen())
        return false;

    //    qDebug() << "before write connects";

    QTimer ticktock;
    QEventLoop waitWrite;
    ticktock.setInterval(mstime);
    ticktock.setSingleShot(false);
    connect((tServer.threads[slot]->socket), SIGNAL(bytesWritten(qint64)), &waitWrite, SLOT(quit()), Qt::DirectConnection);
    connect(&ticktock, SIGNAL(timeout()), &waitWrite, SLOT(quit()), Qt::DirectConnection);
    connect((tServer.threads[slot]->socket), SIGNAL(disconnected()), &waitWrite, SLOT(quit()), Qt::DirectConnection);
    connect(&gameui, SIGNAL(quit_clicked()), &waitWrite, SLOT(quit()), Qt::DirectConnection);

    if(!(tServer.threads[slot]->socket->bytesToWrite()))
        return true;

    ticktock.start();
    waitWrite.exec();

    //    qDebug() << tServer.threads[slot]->socket->bytesToWrite() << " bytes available to write";

    if (!tServer.threads[slot] || (gameStarted && !gameui.isVisible()))
        return false;
    if(!(tServer.threads[slot]->socket->bytesToWrite()))
        return true;
    else
        return false;
}

bool mServer::run_waitGame(int mstime)
{
    QTimer ticktock;
    QEventLoop waitGame;
//    ticktock.setInterval(mstime);
    ticktock.setSingleShot(true);
    connect(&ticktock, SIGNAL(timeout()), &waitGame, SLOT(quit()), Qt::DirectConnection);
    connect(&gameui, SIGNAL(p_check()), &waitGame, SLOT(quit()), Qt::DirectConnection);
    connect(&gameui, SIGNAL(p_fold()), &waitGame, SLOT(quit()), Qt::DirectConnection);
    connect(&gameui, SIGNAL(p_call()), &waitGame, SLOT(quit()), Qt::DirectConnection);
    connect(&gameui, SIGNAL(p_raise(int)), &waitGame, SLOT(quit()), Qt::DirectConnection);
    connect(&gameui, SIGNAL(quit_clicked()), &waitGame, SLOT(quit()), Qt::DirectConnection);

    ticktock.start(mstime);
    waitGame.exec();

    if (gameStarted && !gameui.isVisible())
        return false;
    if(ticktock.isActive())
        return true;
    else
        return false;
}

void mServer::player_toggle_ready(int slot)
{
    QMutex lockthis;

    lockthis.lock();
    if(!(tServer.threads[slot]) || gameStarted)
        return;

    tServer.threads[slot]->socket->readAll(); //empty socket

    if(player_ready[slot]-'0')
    {
        player_ready[slot] = '0';
        if(ui->playerList->item(slot+1))
            delete ui->playerList->item(slot+1);
        ui->playerList->insertItem(slot+1, *(player_stats[slot+1]->pName));
    }
    else
    {
        player_ready[slot] = '1';
        if(ui->playerList->item(slot+1))
            delete ui->playerList->item(slot+1);
        ui->playerList->insertItem(slot+1, *(player_stats[slot+1]->pName) + " -Ready");
    }
    send_lobby_update();
    run_waitGame(100);
    lockthis.unlock();
}

void mServer::on_pbKick_clicked()
{
    QMutex lockthis;

    QByteArray cur_data;
    int slot = ui->playerList->currentRow() - 1;

    if(!(slot+1))
    {
        QMessageBox::information(this, "Notice", "You cannot kick yourself");
        return;
    }
    if(!(tServer.threads[slot]))
        return;

    lockthis.lock();
    //    nameData temp;
    //    temp.slot = -1;
    //    temp.pName = *(tServer.threads[slot]->pName);

    cur_data = "3";
    tServer.threads[slot]->on_readyWrite(cur_data);
    run_waitWrite(slot, 1000);
    tServer.threads[slot]->quit();
    lockthis.unlock();
}

void mServer::on_pbStart_clicked()
{
    QMutex lockthis;

    lockthis.lock();
    int check = 0, check2 = 0;
    for(int i = 0; i < tServer.threads.size(); i++)
    {
        if(tServer.threads[i])
        {
            if(!(player_ready[i] - '0'))
                check++;
            check2++;
        }
    }

    if(check)
    {
        QMessageBox::information(this, "Notice", "There are players who are not ready.");
        return;
    }
    if(!check2)
        return;

    if(gameui.isVisible())
        return;

    gameui.player_name->setText(pName);
    gameui.player_name->setFont(QFont("Ariel", 14));
    gameui.setup_show();

    this->hide();

    QByteArray data = "4\n";
    send_to_all(data);

    gameStarted = true;
    start_game();

    lockthis.unlock();
}

void mServer::on_game_fold_clicked()
{
    player_stats[0]->action = 0;
}

void mServer::on_game_check_clicked()
{
    player_stats[0]->action = 1;
}

void mServer::on_game_call_clicked()
{
    player_stats[0]->action = 2;
}

void mServer::on_game_raise_clicked(int amt)
{
    player_stats[0]->action = 3;
    player_stats[0]->raiseAmt = amt;
}

void mServer::start_game()
{
    //Select the initial dealer position
    mVector<int> dbPicker;
    for (int i = 0; i < player_stats.size(); i++)
    {
        if (player_stats[i])
        {
            dbPicker.push_back(i);
            player_stats[i]->credits = sCred;
            player_stats[i]->isActive = true;
        }
    }

    dealerPos = dbPicker[qrand() % dbPicker.size()];

    //Play rounds until victory conditions have occurred
    do
    {
        start_round();
    } while ((activePlayers > numVic) && gameStarted);


    gameui.p_status->clear();
    for(int i = 0; i < player_stats.size(); i++)
    {
        if(player_stats[i]) //0 folded, 1 check, 2 call, 3 raise, 4 none
        {
            gameui.p_status->addItem(*(player_stats[i]->pName) + " - "
                                     + QString::number(player_stats[i]->credits));
        }
        else
            gameui.p_status->addItem(" ");
    }
    gameui.p_status->addItem("--FINAL SCORE--");

    QString data("1\n");
    data.append(QString::number(cur_player) + "\n");
    for(int i = 0; i < player_stats.size(); i++)
    {
        if(player_stats[i]) //0 folded, 1 check, 2 call, 3 raise, 4 none
        {
            data.append(*(player_stats[i]->pName) + " - "
                        + QString::number(player_stats[i]->credits) + "\n");
        }
        else
            data.append("  \n");
    }
    data.append("--FINAL SCORE--\n");
    data.append(" \n");
    QByteArray sendThis(data.toAscii());
    send_to_all(sendThis);
    run_waitGame(100);

    QByteArray temp = ":\n";
    send_to_all(temp);
    run_waitGame(100);
    QMessageBox::information(this, "Game Ended", "Game has ended thanks for playing!");
}


void mServer::start_round()
{
    /**********************************PRE-FLOP START********************************************/
    QMutex lockthis;
    lockthis.lock();
    gameOver = false;
    pot = 0;
    highRaise = 0;
    int p;
    mVector<ResultStruct*> rsPtr;
    Heap<ResultStruct> resultChecker;

    dealerPos = find_nxt_player(dealerPos);

    for (int i = 0; i < player_stats.size(); i++) //Resets all players last action and number of credits in the pot
    {
        if (player_stats[i])
        {
            player_stats[i]->reset_action();
            player_stats[i]->reset_pot_creds();
            player_stats[i]->raiseAmt = 0;
            player_stats[i]->hand.clear();
        }
    }
    river.clear();

    if (tServer.cur_con - 1) //If we have more than 2 players
    {
        p = find_nxt_player(dealerPos); //Find the small blind

        if (player_stats[p]->credits < sBlind) //If the small blind doesn't have enough for the bet
        {
            pot += player_stats[p]->credits;
            player_stats[p]->credInPot += player_stats[p]->credits;
            player_stats[p]->credits = 0;
        }
        else //If the small blind can pay the forced bet
        {
            pot += sBlind;
            player_stats[p]->credInPot += sBlind;
            player_stats[p]->credits -= sBlind;
        }

        p = find_nxt_player(p); //Find the big blind

        if (player_stats[p]->credits < bBlind) //If the big blind doesn't have enough for the bet
        {
            pot += player_stats[p]->credits;
            player_stats[p]->credInPot += player_stats[p]->credits;
            player_stats[p]->credits = 0;
        }
        else //If the big blind can pay the forced bet
        {
            pot += bBlind;
            player_stats[p]->credInPot += bBlind;
            player_stats[p]->credits -= bBlind;
        }
    }
    else //If we only have 2 players
    {
        if (player_stats[dealerPos]->credits < sBlind) //If the dealer/small blind doesn't have enough for the bet
        {
            pot += player_stats[dealerPos]->credits;
            player_stats[dealerPos]->credInPot += player_stats[dealerPos]->credits;
            player_stats[dealerPos]->credits = 0;
        }
        else //If the dealer/small blind can pay the forced bet
        {
            pot += sBlind;
            player_stats[dealerPos]->credInPot += sBlind;
            player_stats[dealerPos]->credits -= sBlind;
        }

        p = find_nxt_player(dealerPos); //Find the big blind

        if (player_stats[p]->credits < bBlind) //If the big blind doesn't have enough for the bet
        {
            pot += player_stats[p]->credits;
            player_stats[p]->credInPot += player_stats[p]->credits;
            player_stats[p]->credits = 0;
        }
        else //If the big blind can pay the forced bet
        {
            pot += bBlind;
            player_stats[p]->credInPot += bBlind;
            player_stats[p]->credits -= bBlind;
        }
    }


    //Shuffle deck
    cards.NewDeck();
    cards.Shuffle();
    deck.clear();
    Card temp;
    for (int i = 0; i < 52; i++)
    {
        temp = cards.Pop();
        deck.push(temp);
    }
    //At this point, the deck is shuffled and ready for dealing
    //Deal two cards to every player
    Card* tempP;
    for (int i = 0; i < 2; i++)
    {
        for (int i = 0; i < player_stats.size(); i++)
        {
            if(player_stats[i])
            {
                tempP = deck.pop();
                player_stats[i]->hand.push_back(*tempP);
                delete tempP;
            }
        }
    }
    gameui.clear_pocket();
    for(int i = 0; i < 2; i++)
    {
        QString temps1 = ":/images/";
        temps1.append(QString::number(player_stats[0]->hand[i].Idx()));
        temps1.append(".gif");
        gameui.playerhand_front[i] = new QPixmap(temps1);
    }
    gameui.draw_cback();
    //Update every client's game ui
    for (int i = 0; i < tServer.threads.size(); i++)
    {
        if (tServer.threads[i])
        {
            QByteArray data = "7\n";
            data.append(QString::number(player_stats[i+1]->hand[0].Idx()) + "\n");
            data.append(QString::number(player_stats[i+1]->hand[1].Idx()) + "\n");
            tServer.threads[i]->on_readyWrite(data);
        }
    }
    run_waitGame(200);
    for(int i = 0; i < gameui.river.size(); i++)
        gameui.river[i]->setPixmap(*(gameui.cback));
    for(int i = 0; i < tServer.threads.size(); i++)
    {
        if(tServer.threads[i])
        {
            QByteArray data = "8\n";
            tServer.threads[i]->on_readyWrite(data);
        }
    }
    run_waitGame(200);

    gameOver = isGameOver();

    if (!gameOver)
    {

        //Pre-flop betting round
        highRaise = bBlind; //Set the initial highest amt raised to big blind
        p = find_nxt_player(dealerPos); //Go to player after big blind
        p = find_nxt_player(p);

//        qDebug() << p << "starting player";
        start_betting(p);
//        qDebug() << "preflop end";
        /***********************************PRE-FlOP END*********************************/


        /****************************1st BETTING ROUND START********************************/

        if (!gameOver)
        {

            Card* crdPtr = deck.pop(); //Burn a card
            delete crdPtr;
            for (int i = 0; i < 3; i++) //Flop!
            {
                crdPtr = deck.pop();
                river.push_back(*crdPtr);
                delete crdPtr;
            }

            //Send a message to all clients to update their game ui
            //community func 6 flop 0
            for(int i = 0; i < river.size(); i++)
            {
                QString temps1 = ":/images/";
                temps1.append(QString::number(river[i].Idx()));
                temps1.append(".gif");
                (gameui.river[i])->setPixmap(QPixmap(temps1));
            }
            for (int i = 0; i < tServer.threads.size(); i++)
            {
                if (tServer.threads[i])
                {
                    QByteArray data = "6\n";
                    data.append("0\n");
                    for(int j = 0; j < river.size(); j++)
                        data.append(QString::number(river[j].Idx()) + "\n");
                    tServer.threads[i]->on_readyWrite(data);
                }
            }
            run_waitGame(200);

            p = find_nxt_player(dealerPos); //Find small blind
//            qDebug() << p << "starting player";
            start_betting(p);
//            qDebug() << "1st end";
            /****************************1st BETTING ROUND END**********************************/

            /****************************2nd BETTING ROUND START********************************/


            if (!gameOver)
            {
                Card* crdPtr = deck.pop(); //Burn a card
                delete crdPtr;
                //Flip over fourth community card
                crdPtr = deck.pop();
                river.push_back(*crdPtr);
                delete crdPtr;

                //Send a message to all clients to update their game ui
                //turn 1
                if(true)
                {
                    QString temps1 = ":/images/";
                    temps1.append(QString::number(river[3].Idx()));
                    temps1.append(".gif");
                    (gameui.river[3])->setPixmap(QPixmap(temps1));
                }

                for (int i = 0; i < tServer.threads.size(); i++)
                {
                    if (tServer.threads[i])
                    {
                        QByteArray data = "6\n";
                        data.append("1\n");
                        data.append(QString::number(river[3].Idx()) + "\n");
                        tServer.threads[i]->on_readyWrite(data);
                    }
                }
                run_waitGame(200);

                p = find_nxt_player(dealerPos); //Find small blind
//                qDebug() << p << "starting player";
                start_betting(p);
//                qDebug() << "2nd end";

                /****************************2nd BETTING ROUND END**********************************/


                /****************************3rd BETTING ROUND START********************************/

                if (!gameOver)
                {

                    Card* crdPtr = deck.pop(); //Burn a card
                    delete crdPtr;
                    //Flip over fifth community card
                    crdPtr = deck.pop();
                    river.push_back(*crdPtr);
                    delete crdPtr;

                    //Send a message to all clients to update their game ui
                    //river 2
                    if(true)
                    {
                        QString temps1 = ":/images/";
                        temps1.append(QString::number(river[4].Idx()));
                        temps1.append(".gif");
                        (gameui.river[4])->setPixmap(QPixmap(temps1));
                    }

                    for (int i = 0; i < tServer.threads.size(); i++)
                    {
                        if (tServer.threads[i])
                        {
                            QByteArray data = "6\n";
                            data.append("2\n");
                            data.append(QString::number(river[4].Idx()) + "\n");
                            tServer.threads[i]->on_readyWrite(data);
                        }
                    }
                    run_waitGame(200);

                    p = find_nxt_player(dealerPos); //Find small blind
//                    qDebug() << p << "starting player";
                    start_betting(p);

//                    qDebug() << "3rd end";
                    /****************************3rd BETTING ROUND END**********************************/

                    /****************************SHOWDOWN START****************************************/
//                    qDebug() << "before showdown";
                    if (players_not_folded.size() > 1)
                    {
//                        qDebug() << "in showdown if";
                        winCond = 1;
                        ResultStruct* rsPtr3;
                        for (int i = 0; i < player_stats.size(); i++)
                        {
                            if (player_stats[i])
                            {
                                if(player_stats[i]->isActive && player_stats[i]->action)
                                {
                                    // qDebug() << "in for if";
                                    player_stats[i]->eval_hand();
                                    // qDebug() << "player " << i << " result " << player_stats[i]->result;
                                    rsPtr3 = new ResultStruct(player_stats[i]->result, i);
                                    resultChecker.insert(*rsPtr3);
                                    delete rsPtr3;
                                    //  qDebug() << "end for if";
                                }
                            }
                        }
//                        qDebug() << "after for loop1";
                        ResultStruct* rsPtr2 = new ResultStruct();
                        *rsPtr2 = resultChecker.remove();
                        rsPtr.push_back(rsPtr2);
                        rsPtr2 = new ResultStruct(resultChecker.remove());
//                        qDebug() << "before while loop";
                        while(*rsPtr2 == *rsPtr[0])
                        {
//                            qDebug() << "start while loop";
                            rsPtr.push_back(rsPtr2);
                            if (resultChecker.isEmpty())
                            {
                                break;
                            }
                            rsPtr2 = new ResultStruct (resultChecker.remove());
//                            qDebug() << "end of while loop";
                        }
//                        qDebug() << "after while loop";

                        for (int i = 0; i < rsPtr.size(); i++)
                        {
//                            qDebug() << rsPtr[i]->pIndex << " won";
                            player_stats[rsPtr[i]->pIndex]->credits += (pot / rsPtr.size());
                            player_stats[rsPtr[i]->pIndex]->action = 6;
                        }
//                        qDebug() << "end of showdown if";
                    }
//                    qDebug() << "after showdown";

                    /****************************SHOWDOWN END******************************************/
                }
            }
        }
    }




    /****************************SHOW RESULTS START*********************************/

    send_cred_update();
    run_waitGame(100);
    switch(winCond)
    {
    case 0: //folded win
    {
        gameui.p_status->clear();
        for(int i = 0; i < player_stats.size(); i++)
        {
            if(player_stats[i]) //player with action 6 won
            {
                if(!(player_stats[i]->credits))
                    gameui.p_status->addItem(*(player_stats[i]->pName) + " Out");
                else if(!(player_stats[i]->action - 6))
                    gameui.p_status->addItem(*(player_stats[i]->pName) + " Won!");
                else
                    gameui.p_status->addItem(*(player_stats[i]->pName));
            }
            else
                gameui.p_status->addItem(" ");
        }
        gameui.p_status->setCurrentRow(cur_player, QItemSelectionModel::ClearAndSelect);

        QString data("1\n");
        data.append(QString::number(cur_player) + "\n");
        for(int i = 0; i < player_stats.size(); i++)
        {
            if(player_stats[i]) //player with action 6 won
            {
                if(!(player_stats[i]->credits))
                    data.append(*(player_stats[i]->pName) + " Out");
                else if(!(player_stats[i]->action - 6))
                    data.append(*(player_stats[i]->pName) + " Won!");
                else
                    data.append(*(player_stats[i]->pName));

                data.append("\n");
            }
            else
                data.append("  \n");
        }
        data.append(" \n");
        QByteArray sendThis(data.toAscii());
        send_to_all(sendThis);
        break;
    }
    case 1:  //Send hands using player_stats[rsPtr[i]->pIndex]->hand
    {
        gameui.p_status->clear();
        for(int i = 0; i < player_stats.size(); i++)
        {
            if(player_stats[i]) //players with action 6 won
            {
                if(!(player_stats[i]->credits) && player_stats[i]->isActive)
                    gameui.p_status->addItem(*(player_stats[i]->pName) + " " + player_stats[i]->hand[0].val_c()
                                             + player_stats[i]->hand[0].Suit_c() + ", " + player_stats[i]->hand[1].val_c()
                                             + player_stats[i]->hand[1].Suit_c() + " Out");
                else if(!(player_stats[i]->credits))
                    gameui.p_status->addItem(*(player_stats[i]->pName) + " Out");
                else if(!(player_stats[i]->action - 6))
                    gameui.p_status->addItem(*(player_stats[i]->pName) + " " + player_stats[i]->hand[0].val_c()
                                             + player_stats[i]->hand[0].Suit_c() + ", " + player_stats[i]->hand[1].val_c()
                                             + player_stats[i]->hand[1].Suit_c() + " Won!");
                else if(!(player_stats[i]->action))
                    gameui.p_status->addItem(*(player_stats[i]->pName));
                else
                    gameui.p_status->addItem(*(player_stats[i]->pName) + " " + player_stats[i]->hand[0].val_c()
                                             + player_stats[i]->hand[0].Suit_c() + ", " + player_stats[i]->hand[1].val_c()
                                             + player_stats[i]->hand[1].Suit_c() + " Lost");
            }
            else
                gameui.p_status->addItem(" ");
        }
        gameui.p_status->setCurrentRow(cur_player, QItemSelectionModel::ClearAndSelect);

        QString data("1\n");
        data.append(QString::number(cur_player) + "\n");
        for(int i = 0; i < player_stats.size(); i++)
        {
            if(player_stats[i]) //players with action 6 won
            {
                if(!(player_stats[i]->credits) && player_stats[i]->isActive)
                    data.append(*(player_stats[i]->pName) + " " + player_stats[i]->hand[0].val_c()
                                             + player_stats[i]->hand[0].Suit_c() + ", " + player_stats[i]->hand[1].val_c()
                                             + player_stats[i]->hand[1].Suit_c() + " Out");
                else if(!(player_stats[i]->credits))
                    data.append(*(player_stats[i]->pName) + " Out");
                else if(!(player_stats[i]->action - 6))
                    data.append(*(player_stats[i]->pName) + " " + player_stats[i]->hand[0].val_c()
                                + player_stats[i]->hand[0].Suit_c() + ", " + player_stats[i]->hand[1].val_c()
                                + player_stats[i]->hand[1].Suit_c() + " Won!");
                else if(!(player_stats[i]->action))
                    data.append(*(player_stats[i]->pName));
                else
                    data.append(*(player_stats[i]->pName) + " " + player_stats[i]->hand[0].val_c()
                                + player_stats[i]->hand[0].Suit_c() + ", " + player_stats[i]->hand[1].val_c()
                                + player_stats[i]->hand[1].Suit_c() + " Lost");

                data.append("\n");
            }
            else
                data.append("  \n");
        }
        data.append(" \n");
        QByteArray sendThis(data.toAscii());
        send_to_all(sendThis);
        break;
    }
    case 2: //Nobody won! Optional send message
    {
        gameui.p_status->clear();
        for(int i = 0; i < player_stats.size(); i++)
        {
            if(player_stats[i]) //0 folded, 1 check, 2 call, 3 raise, 4 none
            {
                if(!(player_stats[i]->credits))
                    gameui.p_status->addItem(*(player_stats[i]->pName) + " Out");
                else
                    gameui.p_status->addItem(*(player_stats[i]->pName));
            }
            else
                gameui.p_status->addItem(" ");
        }
        gameui.p_status->setCurrentRow(cur_player, QItemSelectionModel::ClearAndSelect);

        QString data("1\n");
        data.append(QString::number(cur_player) + "\n");
        for(int i = 0; i < player_stats.size(); i++)
        {
            if(player_stats[i]) //0 folded, 1 check, 2 call, 3 raise, 4 none
            {
                if(!(player_stats[i]->credits))
                    data.append(*(player_stats[i]->pName) + " Out");
                else
                    data.append(*(player_stats[i]->pName));

                data.append("\n");
            }
            else
                data.append("  \n");
        }
        data.append(" \n");
        QByteArray sendThis(data.toAscii());
        send_to_all(sendThis);
        break;
    }
    case 3:
        break;
    }
    for (int i = 0; i < player_stats.size(); i++) //Put players with no credits left in observer status
    {
        if (player_stats[i])
        {
            if(!(player_stats[i]->credits))
                player_stats[i]->isActive = false;
        }
    }
    gameOver = isGameOver();
    run_waitGame(100);
    QMessageBox::information(this, "Round Ended", "Round ended press ok to unpause.");
    /****************************SHOW RESULTS END***********************************/

//    qDebug() << "end of start round";
    lockthis.unlock();
}




int mServer::find_nxt_player(int start)
{
    int i = start;
    do
    {
        i = (i+1)%player_stats.size();
    } while (!player_stats[i]); //While we haven't found the next player yet
    return i;
}


void mServer::start_betting(int firstPlayer)
{
    QMutex lockthis;

    lockthis.lock();
    int p = firstPlayer;
    highRaiser = p;
    do
    {
        cur_player = p;
        send_lobby_update();
        run_waitGame(100);
//        qDebug() << p << " turn";
        if (player_stats[p])
        {
            if (player_stats[p]->action && (player_stats[p]->credits)) //If the current player has not folded this game round and also has credits
            {

                if ((player_stats[p]->credits + player_stats[p]->credInPot) > highRaise) //If the current player is eligible to raise
                {
                    if (highRaise - player_stats[p]->credInPot)
                    {
                        //Send fold, call, raise request for action //Use highRaise, player_stats[i]->credits
                        if(!p)  //server's turn
                        {
                            gameui.cur_cred = player_stats[p]->credits;
                            gameui.cur_pot_cred = player_stats[p]->credInPot;
                            gameui.cur_high_raise = highRaise;
                            gameui.cur_pot = pot;
                            gameui.on_turn_start_call();
                            if(!run_waitGame(60500))
                            {
                                qDebug() << "wait game false";
                                emit this->on_game_fold_clicked();
                            }
                            run_waitGame(10);
                        }
                        else    //client turn
                        {
                            QByteArray temp = "5\n";
                            temp.append("0\n");
                            temp.append(QString::number(player_stats[p]->credits) + "\n");
                            temp.append(QString::number(player_stats[p]->credInPot) + "\n");
                            temp.append(QString::number(highRaise) + "\n");
                            temp.append(QString::number(pot) + "\n");
                            tServer.threads[p-1]->on_readyWrite(temp);

                            if(run_waitRead(p-1, 60500))
                            { //read response
                                QByteArray tempread = tServer.threads[p-1]->socket->readLine();
                                player_stats[p]->action = tempread[0] - '0';
                                if(!(player_stats[p]->action - 3))
                                {
                                    tempread = tServer.threads[p-1]->socket->readLine();
                                    while(!(tempread[tempread.size()-1] - '\n'))
                                        tempread.chop(1);
                                    player_stats[p]->raiseAmt = tempread.toInt();
                                }
                                tServer.threads[p-1]->socket->readAll(); //empty socket
                            }
                            else
                            {
                                if(player_stats[p] && !gameOver)
                                    player_stats[p]->action = 0;
                                else
                                    return;
                            }
                        }
                    }
                    else
                    {
                        //Send fold, check, raise request for action //Use highRaise, player_stats[i]->credits
                        if(!p)  //server's turn
                        {
                            gameui.cur_cred = player_stats[p]->credits;
                            gameui.cur_pot_cred = player_stats[p]->credInPot;
                            gameui.cur_high_raise = highRaise;
                            gameui.cur_pot = pot;
                            gameui.on_turn_start_check();
                            if(!run_waitGame(60500))
                            {
                                qDebug() << "wait game false";
                                emit this->on_game_fold_clicked();
                            }
                            run_waitGame(10);
                        }
                        else    //client turn
                        {
                            QByteArray temp = "5\n";
                            temp.append("2\n");
                            temp.append(QString::number(player_stats[p]->credits) + "\n");
                            temp.append(QString::number(player_stats[p]->credInPot) + "\n");
                            temp.append(QString::number(highRaise) + "\n");
                            temp.append(QString::number(pot) + "\n");
                            tServer.threads[p-1]->on_readyWrite(temp);

                            if(run_waitRead(p-1, 60500))
                            { //read response
                                QByteArray tempread = tServer.threads[p-1]->socket->readLine();
                                player_stats[p]->action = tempread[0] - '0';
                                if(!(player_stats[p]->action - 3))
                                {
                                    tempread = tServer.threads[p-1]->socket->readLine();
                                    while(!(tempread[tempread.size()-1] - '\n'))
                                        tempread.chop(1);
                                    player_stats[p]->raiseAmt = tempread.toInt();
                                }
                                tServer.threads[p-1]->socket->readAll(); //empty socket
                            }
                            else
                            {
                                if(player_stats[p] && !gameOver)
                                    player_stats[p]->action = 0;
                                else
                                    return;
                            }
                        }
                    }
                }
                else
                {
                    //Send fold, call request for action
                    if(!p)  //server's turn
                    {
                        gameui.cur_cred = player_stats[p]->credits;
                        gameui.cur_pot_cred = player_stats[p]->credInPot;
                        gameui.cur_high_raise = highRaise;
                        gameui.cur_pot = pot;
                        gameui.on_turn_start_allin();
                        if(!run_waitGame(60500))
                        {
                            qDebug() << "wait game false";
                            emit this->on_game_fold_clicked();
                        }
                        run_waitGame(10);
                    }
                    else    //client turn
                    {
                        QByteArray temp = "5\n";
                        temp.append("1\n");
                        temp.append(QString::number(player_stats[p]->credits) + "\n");
                        temp.append(QString::number(player_stats[p]->credInPot) + "\n");
                        temp.append(QString::number(highRaise) + "\n");
                        temp.append(QString::number(pot) + "\n");
                        tServer.threads[p-1]->on_readyWrite(temp);

                        if(run_waitRead(p-1, 60500))
                        { //read response
                            QByteArray tempread = tServer.threads[p-1]->socket->readLine();
                            player_stats[p]->action = tempread[0] - '0';
                            if(!(player_stats[p]->action - 3))
                            {
                                tempread = tServer.threads[p-1]->socket->readLine();
                                while(!(tempread[tempread.size()-1] - '\n'))
                                    tempread.chop(1);
                                player_stats[p]->raiseAmt = tempread.toInt();
                            }
                            tServer.threads[p-1]->socket->readAll(); //empty socket
                        }
                        else
                        {
                            if(player_stats[p] && !gameOver)
                                player_stats[p]->action = 0;
                            else
                                return;
                        }
                    }
                }
                //Receive reply and update player_stats[p]->action and possibly player_stats[p]->raiseAmt
                if(!(player_stats[p]) || gameOver)
                    return;

                switch (player_stats[p]->action)
                {   //0 fold 1 check
                case 0:
                    break;
                case 1:
                    break;
                case 2: //Current player decided to call
                    if (player_stats[p]->credits <= (highRaise-player_stats[p]->credInPot)) //If the current player doesn't have enough for a call
                    {
                        pot += player_stats[p]->credits;
                        player_stats[p]->credits = 0;
                        player_stats[p]->credInPot += player_stats[p]->credits;
                    }
                    else //If the current player has enough for a call
                    {
                        pot += (highRaise - player_stats[p]->credInPot);
                        player_stats[p]->credits -= (highRaise - player_stats[p]->credInPot);
                        player_stats[p]->credInPot = highRaise;
                    }
                    break;
                case 3: //Current player decided to raise
                    highRaise = player_stats[p]->raiseAmt;
                    pot += (highRaise - player_stats[p]->credInPot);
                    player_stats[p]->credits -= (highRaise - player_stats[p]->credInPot);
                    player_stats[p]->credInPot = highRaise;
                    highRaiser = p;
                    break;
                case 4:
                    break;
                }
            }
        }
        send_cred_update();
        run_waitGame(100);
        if (isGameOver())
        {
            gameOver = true;
            break;
        }
        p = find_nxt_player(p);

        if(!(player_stats[highRaiser]))
            highRaiser = find_nxt_player(highRaiser);
    } while (p != highRaiser);
    lockthis.unlock();
}


bool mServer::isGameOver()
{
    foldTally = 0;
    activePlayers = 0;
    players_not_folded.clear();
    for (int i = 0; i < player_stats.size(); i++)
    {
        if (player_stats[i])
        {
            if (player_stats[i]->isActive)
            {
                activePlayers++;//If player is "active"
                if (!player_stats[i]->action) //If player has folded
                {
                    foldTally++;
                }
                else
                {
                    players_not_folded.push_back(i);
                }
            }
        }
    }
    if(!gameui.isVisible())
    {
        winCond = 3;
        gameStarted = false;
        return true;
    }
    if ((players_not_folded.size() <= 1) || (activePlayers <= numVic)) //If all but one player has folded or if no players are active
    {
        if (players_not_folded.size() == 1)
        {
            winCond = 0;
            player_stats[players_not_folded[0]]->credits += pot;
        }
        else
        {
            winCond = 2;
            gameStarted = false;
        }
        return true;
    }
    return false;
}


