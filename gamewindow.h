#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include "mvector.h"
#include "qlabelclick.h"

namespace Ui {
    class gameWindow;
}
//graphic display window
class gameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit gameWindow(QWidget *parent = 0);
    ~gameWindow();
    void setup_show(); //constructor show function


signals:
    void quit_clicked(); //quit button clicked
    void p_fold(); //signal fold
    void p_check(); //signal check
    void p_call(); //signal call
    void p_raise(int amount); //signal raise

public slots:
    void on_turn_start_call(); //shows call, fold, raise
    void on_turn_start_check(); //shows check, fold, raise
    void on_turn_start_allin(); //shows call, fold only
    void on_turn_end();     //hides all buttons
    void hand1_clicked(); //left card clicked
    void hand2_clicked(); //right card clicked
    void draw_cback();  //draw card backs to screen
    void on_pbQuit_clicked(); //slot for quit clicked
    void clear_pocket();

private slots:
    void on_pbRaise_clicked(); //slot for raise
    void on_pbFold_clicked(); //slot for fold
    void on_pbCall_clicked(); //slot for call
    void on_check_clicked(); //slot for check

private:
    void update_cred_labels(); //update credit labels

    Ui::gameWindow *ui;
    mVector<QLabelClick*> playerhand; //clickable card labels
    mVector<QLabel*> river; //community cards
    QPixmap *cback, *cblank; //card back pixmap and blank pixmap
    mVector<QPixmap*> playerhand_front; //pixmap for front of pocket cards
    QLabel *credit_lab, *pot_cred_lab, *high_raise_lab, *pot_lab; //labels for credit values
    QLabel *help_flip, *help_river, *player_name; //labels for tips
    QPushButton* check; //check push button
    int cur_cred, cur_pot_cred, cur_high_raise, cur_pot; //current values for credit
    bool ph1, ph2; //if true card is currently face up
    QListWidget* p_status; //pointer to status_view
    QTimer minute;

    friend class mClient;
    friend class mServer;
};

#endif // GAMEWINDOW_H
