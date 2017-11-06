#include <iostream>
#include "General.hpp"
#include "Market.hpp"
#include "PlayerCards.hpp"

using namespace std;

Market::Market(){
    cout << "create " << MARKET << endl;
}

Market::~Market(){
    cout << "delete " << MARKET << endl;
}

// +1カード、+1アクション、+1購入、+1コイン
ActionDone Market::action(PLAYERCARD_CONCRETECARD_IO *p){
    cout << "action " << MARKET << endl;
    p->resultData.drawCard = 1;
    p->resultData.data.action = 1;
    p->resultData.data.buy = 1;
    p->resultData.data.coin = 1;
    return ACTIONDONE;
}

const string* Market::who(){
    return &MARKET;
}
