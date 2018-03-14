#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include "mvector.h"
#include "CardLib/Card.h"

class Player
{
public:
    Player(QString* pn = NULL, mVector<Card>* rvr = NULL, int sc = 1);

    QString* pName;
    int raiseAmt;
    int action; //0-3 for folded, checked, called, raised
    mVector<Card> hand;
    mVector<Card>* river;
    int credits;
    int credInPot;
    bool isActive;

    void eval_hand();

    bool hasFlush(); //Helper function for evaluating a hand
    bool hasSFlush(); //Helper function for evaluating a hand
    void reset_containers(); //Resets containers used for evaluating a hand
    void reset_action(); //Resets the action last taken by the player
    //(should be called at the beginning of every betting round)
    void reset_pot_creds(); //Resets the # of credits this player has invested in the pot
    //(should be called at the beginning of every game round)

    double result; //The player's last 5-card hand
    mVector<int> straightChecker[14]; //Used during straight checking.
    mVector<int> flushChecker[4]; //Used during flush checking.
    mVector<int> ofAKind[4]; //Used to check anything but a straight, flush, or straight flush
    int straightCounter; //Used during straight and straight flush checking.
    bool hadStraight; //Remembers if we had a straight or not.
    int flushSuit; //Used during flush checking. Should have a value of 0-3 if we have a flush. -1 otherwise
    int sfHigh; //Used during checking for straight flush. Holds the value of the high card (1-13)

};

#endif // PLAYER_H
