#include "player.h"

Player::Player(QString *pn, mVector<Card>* rvr, int sc)
{
    pName = pn;
    river = rvr;
    raiseAmt = 0;
    reset_containers();
    credits = sc;
    action = 4;
    credInPot = 0;
    flushSuit = -1;
    isActive = true;
}


void Player::eval_hand()
{
    reset_containers();

//    qDebug() << "start of eval";
    //Update straightChecker
    for (int i = 0; i < 2; i++)
    {
//        qDebug() << "start of for eval";
        if (hand[i].LoVal() - 1) //If this card is not an Ace
        {
            straightChecker[hand[i].LoVal() - 1].push_back(hand[i].Suit());
        }
        else //If this card is an Ace
        {
            straightChecker[0].push_back(hand[i].Suit());
            straightChecker[13].push_back(hand[i].Suit());
        }
//        qDebug() << "end of for eval";
    }
//    qDebug() << "before second for eval";
    for (int i = 0; i < river->size(); i++)
    {
        if ((*river)[i].LoVal() - 1) //If this card is not an Ace
        {
            straightChecker[(*river)[i].LoVal() - 1].push_back((*river)[i].Suit());
        }
        else //If this card is an Ace
        {
            straightChecker[0].push_back((*river)[i].Suit());
            straightChecker[13].push_back((*river)[i].Suit());
        }
    }

//    qDebug() << "after second for eval";


    //Check for a straight and prepare flushChecker and ofAKind for checking for other hands
    for (int i = 0; i < 14; i++)
    {
//        qDebug() << "start of for3 eval";
        if (straightChecker[i].size()) //If we have a straight of 1-7
        {
            straightCounter++;
            if (straightCounter > 4) //If we have a straight of 5-7
            {
                result = 5 + (double)(i) / 100;
                hadStraight = true;
            }
            //Preparation for checking other hands
            for (int j = 0; j < straightChecker[i].size(); j++)
            {
                if(i-13)
                    flushChecker[straightChecker[i][j]].push_back(i);
            }
            if (i) //If the value of the card is not "low" ace (eliminating the duplicate)
            {
                ofAKind[4 - straightChecker[i].size()].push_back(i);
            }
        }
        else //If we have broken a streak
        {
            straightCounter = 0;
        }
//        qDebug() << "end of for3 eval";
    }



//    qDebug() << "after for3 eval";
    //Check for remaining hands
    if (!ofAKind[0].empty()) //If we have a four of a kind
    {
//        qDebug() << "in if1 check eval";
        int temp = ofAKind[0][0];
        int kicker;
        if (ofAKind[1].empty()) //If we have a 4-2-1 or 4-1-1-1
        {
            if (ofAKind[2].empty()) //If we have a 4-1-1-1
            {
                kicker = ofAKind[3].back();
            }
            else //If we have a 4-2-1
            {
                kicker = max(ofAKind[2].back(),ofAKind[3].back());
            }
        }
        else //If we have a 4-3
        {
            kicker = ofAKind[1][0];
        }
        result = 8 + (double)(temp) / 100 + (double)(kicker) / 10000;
    }
    else if (hasFlush()) //If we have a flush
    {
//        qDebug() << "in else if check eval";
        if (hadStraight) //If we have a straight flush
        {
            if(hasSFlush())
                result = 9 + (double)(sfHigh) / 100;
        }
        else //If we just have a regular flush
        {
            int suitSize = flushChecker[flushSuit].size();
            result = 6 + (double)flushChecker[flushSuit][suitSize-1] / 100
                    + (double)flushChecker[flushSuit][suitSize-2] / 10000
                    + (double)flushChecker[flushSuit][suitSize-3] / 1000000
                    + (double)flushChecker[flushSuit][suitSize-4] / 100000000
                    + (double)flushChecker[flushSuit][suitSize-5] / 10000000000;
        }
    }
    else if (!ofAKind[1].empty()) //If we have a three of a kind
    {
//        qDebug() << "in else if of a kind empty";
        if (!ofAKind[2].empty()) //If we have a full house (i.e. 3-3-1 or 3-2-2 or 3-2-1-1)
        {
            if (ofAKind[2].size() == 2) //If we have 3-3-1
            {
                result = 7 + (double)(ofAKind[1][1]) / 100 + (double)(ofAKind[1][0]) / 10000;
            }
            else //If we have 3-2-2 or 3-2-1-1
            {
                result = 7 + (double)(ofAKind[1][0]) / 100 + (double)(ofAKind[2].back()) /10000;
            }
        }
        else if (!hadStraight) //If we just have a regular three of a kind (i.e. 3-1-1-1-1)
        {
            result = 4 + (double)(ofAKind[1][0]) / 100 + (double)(ofAKind[3][ofAKind[3].size()-1]) / 10000
                    + (double)(ofAKind[3][ofAKind[3].size()-2]) / 1000000;
        }
    }
    else if (!hadStraight) //If we haven't got anything higher than a two pair
    {
//        qDebug() << "in else if has straight";
        if (!ofAKind[2].empty()) //If we have a pair
        {
            if (ofAKind[2].size() == 3) //If we have a two pair 2-2-2-1
            {
                result = 3 + (double)(ofAKind[2][ofAKind[2].size()-1]) / 100
                        + (double)(ofAKind[2][ofAKind[2].size()-2]) / 10000
                        + (double)(max(ofAKind[2][ofAKind[2].size()-3],ofAKind[3][0])) / 1000000;
            }
            else if (ofAKind[2].size() == 2) //If we have a two pair 2-2-1-1-1
            {
                result = 3 + (double)(ofAKind[2][ofAKind[2].size()-1]) / 100
                        + (double)(ofAKind[2][ofAKind[2].size()-2]) / 10000
                        + (double)(ofAKind[3][ofAKind[3].size()-1]) / 1000000;
            }
            else //If we have a regular pair (i.e. 2-1-1-1-1-1)
            {
                result = 2 + (double)(ofAKind[2][0]) / 100 + (double)(ofAKind[3][ofAKind[3].size()-1]) / 10000
                        + (double)(ofAKind[3][ofAKind[3].size()-2]) / 1000000
                        + (double)(ofAKind[3][ofAKind[3].size()-3]) / 100000000;
            }
        }
        else //If we have just a high card (i.e. 1-1-1-1-1-1-1)
        {
            result = 1 + (double)(ofAKind[3][6]) / 100 + (double)(ofAKind[3][5]) / 10000
                    + (double)(ofAKind[3][4]) / 1000000 + (double)(ofAKind[3][3]) / 100000000
                    + (double)(ofAKind[3][2]) / 10000000000;
        }
    }
//    qDebug() << "before reset container in eval";
//    qDebug() << "end of eval";
}








//Check for a flush and set flushSuit if we have a flush
bool Player::hasFlush()
{
//    qDebug() << "in has flush";
//    int suitSize;
    for (int i = 0; i < 4; i++)
    {
//        qDebug() << "start of flush for";
        if(flushChecker[i].size() > 4) //If the number of cards for this suit is 5 or more
        {
            flushSuit = i;
            return true;
        }
//        suitSize = flushChecker[i].size();
//        if (suitSize) //If the number of cards for this suit is more than 0
//        { //(3 + (bool)flushChecker[i][0])
//            if (suitSize > 4)
//            {
//                flushSuit = i;
//                return true;
//            }
//        }
//        qDebug() << "end of flush for";
    }
//    qDebug() << "end of flush";
    return false;
}



//Check for a straight flush and set sfHigh if we have a straight flush
bool Player::hasSFlush()
{
//    qDebug() << "start of sflush check";
    bool sf = false;
    int prev;
    if (flushSuit+1) //If we have a straight and a flush, check for a straight flush
    {
        if(!(flushChecker[flushSuit][0]))
            flushChecker[flushSuit].push_back(13);
//        qDebug() << "in sflush if";
        prev = flushChecker[flushSuit][0];
        straightCounter = 1;
//        qDebug() << "before sflush for";
        for (int i = 1; i < flushChecker[flushSuit].size(); i++)
        {
//            qDebug() << "start sflush for";
            if ((flushChecker[flushSuit][i]-prev)-1) //If the next card does not extend the straight
            {
                straightCounter = 1;
            }
            else //If the next card extends the straight
            {
                straightCounter++;
                if (straightCounter > 4) //If we have a straight flush
                {
                    sf = true;
                    sfHigh = flushChecker[flushSuit][i];
                }
            }
            prev = flushChecker[flushSuit][i];
//            qDebug() << "end sflush for";
        }
//        qDebug() << "end of sflush if";
    }
//    qDebug() << "end of sflush";
    return sf;
}




void Player::reset_containers()
{
//    qDebug() << "in reset containers";
    result = 0;
    straightCounter = 0;
    hadStraight = false;
    flushSuit = -1;
    sfHigh = -1;
//    qDebug() << "before reset_cont for1";
    for (int i = 0; i < 4; i++)
    {
        straightChecker[i].clear();
        flushChecker[i].clear();
        ofAKind[i].clear();
    }
//    qDebug() << "before reset cont for2";
    for (int i = 4; i < 14; i++)
    {
        straightChecker[i].clear();
    }
//    qDebug() << "end of reset containers";
}


void Player::reset_action()
{
    action = 4;
}


void Player::reset_pot_creds()
{
    credInPot = 0;
}
