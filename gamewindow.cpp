#include "gamewindow.h"
#include "ui_gamewindow.h"

gameWindow::gameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gameWindow)
{
    ui->setupUi(this);
    //set status view pointer
    p_status = ui->status_view;
    player_name = ui->playerName;
    //set pixmaps for reuse
    cback = new QPixmap(":/images/back.gif");
    cblank = new QPixmap(":/images/blank.gif");
    playerhand_front.resize(2, NULL);
    //set up check push button
    check = new QPushButton(ui->widget);
    check->setText("Check");
    check->move(607,25);
    check->setMinimumSize(80,30);
    check->setMaximumSize(80,30);
    connect(check, SIGNAL(clicked()), this, SLOT(on_check_clicked()));
    //set up current credit label
    credit_lab = new QLabel(ui->cardView);
    credit_lab->move(10, 570);
    credit_lab->setMinimumWidth(180);
    credit_lab->setMaximumWidth(180);
    //setup current amount in pot label
    pot_cred_lab = new QLabel(ui->cardView);
    pot_cred_lab->move(200, 570);
    pot_cred_lab->setMinimumWidth(250);
    pot_cred_lab->setMaximumWidth(250);
    //setup current highest raise label
    high_raise_lab = new QLabel(ui->cardView);
    high_raise_lab->move(560, 570);
    high_raise_lab->setMinimumWidth(200);
    high_raise_lab->setMaximumWidth(200);
    //setup pot label
    pot_lab = new QLabel(ui->cardView);
    pot_lab->move(10, 530);
    pot_lab->setMinimumWidth(200);
    pot_lab->setMaximumWidth(200);
    //setup tip labels
    help_flip = new QLabel(ui->cardView);
    help_river = new QLabel(ui->cardView);
    help_flip->move(248, 520);
    help_river->move(175, 240);

    QFont cred_f("Ariel", 14); //set font
    credit_lab->setText("Credit: 000000000");
    credit_lab->setFont(cred_f);

    help_flip->setText("(Click cards to show or hide them)");
    help_river->setText("(Above cards will be shown when they are dealt)");
    help_flip->setFont(cred_f);
    help_river->setFont(cred_f);

    pot_cred_lab->setText("Amount you have in pot: 000000000");
    pot_cred_lab->setFont(cred_f);

    high_raise_lab->setText("Must match: 000000000");
    high_raise_lab->setFont(cred_f);

    pot_lab->setText("Current Pot: ");
    pot_lab->setFont(cred_f);

    playerhand.resize(2, NULL); //resize vectors to respective sizes
    river.resize(5, NULL);
    for(int i = 0; i < river.size() ; i++) //initialize slots in vectors
    {
        river[i] = new QLabel(ui->cardView);
        river[i]->move((i*(155)+15), 20); //move to correct positions
    }
    for(int i = 0; i < playerhand.size(); i++)
    {
        playerhand[i] = new QLabelClick(ui->cardView);
        playerhand[i]->move((i*155)+248, 300);
    }
    //connect clicked signals for flipping cards
    connect(playerhand[0], SIGNAL(clicked()), this, SLOT(hand1_clicked()));
    connect(playerhand[1], SIGNAL(clicked()), this, SLOT(hand2_clicked()));

    connect(&minute, SIGNAL(timeout()), this, SLOT(on_turn_end()));
    minute.setSingleShot(false);
    minute.setInterval(60000);
    //prevent status_view from being selectable
    ui->status_view->setSelectionMode(QAbstractItemView::NoSelection);
}
//destructor
gameWindow::~gameWindow()
{
    delete credit_lab;
    delete cback;
    delete cblank;
    delete help_flip;
    delete help_river;
    delete pot_cred_lab;
    for(int i = 0; i < river.size(); i++)
    {
        delete river[i];
        river[i] = NULL;
    }
    for(int i = 0; i < playerhand.size(); i++)
    {
        delete playerhand[i];
        playerhand[i] = NULL;
    }
    clear_pocket();
    delete ui;
}
//constructor show function
void gameWindow::setup_show()
{
    for(int i = 0; i < river.size(); i++) //set community cards to back.gif
    {
        river[i]->setPixmap(*cback);
    }

    for(int i = 0; i < playerhand.size(); i++) //set hand to blank
    {
        playerhand[i]->setPixmap(*cblank);
    }

    ph1 = false; //start face down
    ph2 = false;

    credit_lab->setText("Credit: --");
    pot_cred_lab->setText("Amount you have in pot: --");
    high_raise_lab->setText("Must match: --");
    pot_lab->setText("Current Pot: --");
//    //once cards received
//    draw_cback();
//    //testing stuff
//    playerhand_front[0] = new QPixmap(":/images/" + QString::number(50) + ".gif");
//    playerhand_front[1] = new QPixmap(":/images/" + QString::number(7) + ".gif");
    on_turn_end();

    this->show();
}
//shows call, fold, raise
void gameWindow::on_turn_start_call()
{
    if(cur_cred+cur_pot_cred <= cur_high_raise) //setup spinbox min max
    {
        ui->sbCred->setMinimum(cur_cred+cur_pot_cred);
        ui->sbCred->setMaximum(cur_cred+cur_pot_cred);
    }
    else
    {
        ui->sbCred->setMinimum(cur_high_raise + 1);
        ui->sbCred->setMaximum(cur_cred + cur_pot_cred);
    }

    update_cred_labels(); //show updated credit values

    ui->pbCall->show(); //show buttons
    ui->pbFold->show();
    ui->pbRaise->show();
    ui->sbCred->show();
    minute.start();
}
//shows check, fold, raise
void gameWindow::on_turn_start_check()
{
    if(cur_cred+cur_pot_cred <= cur_high_raise) //setup spinbox min max
    {
        ui->sbCred->setMinimum(cur_cred+cur_pot_cred);
        ui->sbCred->setMaximum(cur_cred+cur_pot_cred);
    }
    else
    {
        ui->sbCred->setMinimum(cur_high_raise + 1);
        ui->sbCred->setMaximum(cur_cred + cur_pot_cred);
    }

    update_cred_labels(); //show updated credit values

    check->show();      //show buttons
    ui->pbFold->show();
    ui->pbRaise->show();
    ui->sbCred->show();
    minute.start();
}
//shows call, fold only
void gameWindow::on_turn_start_allin()
{
    update_cred_labels();

    ui->pbCall->show(); //shows only these 2 buttons
    ui->pbFold->show();
    minute.start();
}
//hides all buttons
void gameWindow::on_turn_end()
{
    minute.stop();
    check->hide();      //hides all buttons
    ui->pbCall->hide();
    ui->pbFold->hide();
    ui->pbRaise->hide();
    ui->sbCred->hide();
}
//slot for quit clicked
void gameWindow::on_pbQuit_clicked()
{
    this->close();
    clear_pocket();
    emit this->quit_clicked();
}

void gameWindow::clear_pocket()
{
    for(int i = 0; i < playerhand_front.size(); i++) //delete player hand front values
    {
        if(playerhand_front[i])
        {
            delete playerhand_front[i];
            playerhand_front[i] = NULL;
        }
    }
}
//left card clicked flips card
void gameWindow::hand1_clicked()
{
    if(playerhand_front[0])
    {
        if(ph1)
        {
            playerhand[0]->setPixmap(*cback);
            ph1 = false;
        }
        else
        {
            playerhand[0]->setPixmap(*(playerhand_front[0]));
            ph1 = true;
        }
    }
    else
    {
        playerhand[0]->setPixmap(*cblank);
        ph1 = false;
    }

}
//right card clicked flips card
void gameWindow::hand2_clicked()
{
    if(playerhand_front[1])
    {
        if(ph2)
        {
            playerhand[1]->setPixmap(*cback);
            ph2 = false;
        }
        else
        {
            playerhand[1]->setPixmap((*playerhand_front[1]));
            ph2 = true;
        }
    }
    else
    {
        playerhand[1]->setPixmap(*cblank);
        ph2 = false;
    }
}
//draw card backs to screen
void gameWindow::draw_cback()
{
    playerhand[0]->setPixmap(*cback);
    playerhand[1]->setPixmap(*cback);
    ph1 = false;
    ph2 = false;
}
//slot for raise
void gameWindow::on_pbRaise_clicked()
{               //safety incase somehow player enters more than allowed
    if((cur_cred - (ui->sbCred->value() - cur_pot_cred)) < 0)
    {
        QMessageBox::information(this, "Notice", "You do not have enough credits to raise.");
        return;
    }

    on_turn_end(); //update credit values
    cur_pot += (ui->sbCred->value() - cur_pot_cred);
    cur_cred -= (ui->sbCred->value() - cur_pot_cred);
    cur_pot_cred += (ui->sbCred->value() - cur_high_raise);
    cur_high_raise = 0;

    update_cred_labels();
    emit p_raise(ui->sbCred->value());
}
//slot for fold
void gameWindow::on_pbFold_clicked()
{
    on_turn_end();  //if fold just send fold
    cur_high_raise = 0;

    update_cred_labels();
    emit p_fold();
}
//slot for call
void gameWindow::on_pbCall_clicked()
{                       //update credit values for call
    if((cur_cred + cur_pot_cred) <= cur_high_raise) //if all in
    {
        cur_pot_cred += cur_cred;
        cur_pot += cur_cred;
        cur_cred = 0;
    }
    else        //not all in
    {
        cur_pot += (cur_high_raise - cur_pot_cred);
        cur_cred -= (cur_high_raise - cur_pot_cred);
        cur_pot_cred += (cur_high_raise - cur_pot_cred);
    }
    on_turn_end();
    cur_high_raise = 0;

    update_cred_labels();
    emit p_call();
}
//slot for check
void gameWindow::on_check_clicked()
{
    on_turn_end();  //if check is clicked just send check
    cur_high_raise = 0;

    update_cred_labels();
    emit p_check();
}
//update credit labels
void gameWindow::update_cred_labels() //displays credit amounts using QLabel
{
    credit_lab->setText("Credit: " + QString::number(cur_cred));
    pot_cred_lab->setText("Amount you have in Pot: " + QString::number(cur_pot_cred));
    pot_lab->setText("Current Pot: " + QString::number(cur_pot));

    if(!cur_high_raise)
        high_raise_lab->setText("Must match: --");
    else
        high_raise_lab->setText("Must match: " + QString::number(cur_high_raise));
}
