#ifndef MCLIENT_H
#define MCLIENT_H

#include <QDialog>
#include <QTcpSocket>
#include <QString>
#include <QMessageBox>
#include <QAbstractSocket>
#include <QMutex>
#include <QFont>
#include <conio.h>
#include <cstdlib>
#include "gamewindow.h"

using namespace std;

namespace Ui {
class mClient;
}
//client for texas holdem game
class mClient : public QDialog
{
    Q_OBJECT
    typedef void (mClient::*fpc)(); //typedef for function pt
    
public:
    explicit mClient(QWidget *parent = 0);
    ~mClient();

signals:
    void back_clicked(); //signal for back clicked
    
private slots:
    void on_connected(); //slot for on connect
    void on_disconnected(); // slot for disconnect
    void on_readyRead(); //slot triggers once socket::readyRead triggered
    void on_lineEdit_textChanged(const QString &arg1); //line edit slot holds ip address
    void on_pbCon_clicked();    //on push button connect clicked
    void on_pbBack_clicked(); //on back button clicked

    void on_pbReady_clicked(); //when ready button clicked
    void p_call();  //player clicked call
    void p_raise(int amount); //player clicked raise
    void p_fold();  //player clicked fold
    void p_check(); //player clicked check

private:
    void send_name(); //sends name to server
    void update_lobby(); //updates lobby screen
    void name_in_use(); //name in use popup
    void player_kicked(); //kicked popup
    void start_game(); //start game sent from server
    void clean_up(); //clean up lobby
    void p_start_turn(); //server signalled player to start their turn
    void receive_community(); //receive community card
    void receive_flop(); //receive flop redirected from receive_community
    void receive_turn(); //receive turn card redirected from receive_community
    void receive_river(); //receive river card redirected from receive_community
    void receive_pocket(); //receive pocket cards
    void reset_screen(); //reset community cards
    void update_cred(); //update credit labels
    void game_ended();

    Ui::mClient *ui;
    QString host_ip;
    gameWindow gameui;
    QString pName;
    QTcpSocket* socket;
    fpc inst[11];   //array of function ptrs
    bool gameStarted;

    friend class MainWindow;
};

#endif // MCLIENT_H
