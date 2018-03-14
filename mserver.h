#ifndef MSERVER_H
#define MSERVER_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QMessageBox>
#include <QHostInfo>
#include <QTextStream>
#include <conio.h>
#include <cstdlib>
#include <QEventLoop>
#include <QTimer>
#include <QThread>
#include <QByteArray>
#include <QTime>
#include <QtGlobal>
#include <QFont>
#include "mthread.h"
#include "mqtcpserver.h"
#include "gamewindow.h"
#include "namedata.h"
#include "btree.h"
#include "CardLib/CardStack.h"
#include "CardLib/Card.h"
#include "player.h"
#include "Stack.h"
#include "resultstruct.h"
#include "Heap.h"

using namespace std;

namespace Ui {
class mServer;
}
//server gui class, also runs the game
class mServer : public QDialog
{
    Q_OBJECT
    typedef void (mServer::*fpserv)(int);

public:
    explicit mServer(QWidget *parent = 0);
    ~mServer();
    void startServer();

signals:
    void back_clicked(); //if back button clicked emit this

private slots:
    void on_back_clicked(); //clean up function when back clicked
    void error_msg(QString errorString);  //pass along error msg
    void on_create_player(int slot); //triggers when new connection
    void on_player_drop(int slot); //on player disconnect
    void on_ready_read(int slot); //on ready read only used for ready check
    void on_pbKick_clicked(); //on kick clicked
    void on_pbStart_clicked(); //on start clicked
    void on_game_fold_clicked(); //on fold slot
    void on_game_check_clicked(); //check button slot
    void on_game_call_clicked(); //call button slot
    void on_game_raise_clicked(int amt); //raise button slot

private:
    void send_lobby_update(); //send update to all clients for lobby
    void send_cred_update(); //update credit labels for gameui
    void send_to_all(QByteArray &data); //send data to all clients
    bool run_waitRead(int slot, int mstime); //custom wait function
    bool run_waitWrite(int slot, int mstime); //custom wait function
    bool run_waitGame(int mstime); //custom wait function
    void player_toggle_ready(int slot); //on client toggled ready
    void start_game(); //start game function
    void start_round(); //game round function
    int find_nxt_player(int start); //finds next existing player given start
    void start_betting(int firstPlayer);
    bool isGameOver();

    Ui::mServer *ui;
    int numPlay, numVic, sCred, bBlind, sBlind, pot, dealerPos;
    int highRaiser, highRaise, foldTally, activePlayers, winCond;
    int cur_player;
    mVector<Player*> player_stats;
    mVector<int> players_not_folded;
    QByteArray player_ready;
    QString pName;
    bTree<nameData> nTree;
    CardStack cards;
    Stack<Card> deck;
    mQTcpServer tServer;
    gameWindow gameui;
    fpserv inst_serv[10];
    mVector<Card> river;
    bool gameOver, gameStarted;


    friend class MainWindow;
};

#endif // MSERVER_H
