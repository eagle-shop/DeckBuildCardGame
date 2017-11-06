#include <iostream>
#include "General.hpp"
#include "Woodcutter.hpp"
#include "PlayerCards.hpp"

using namespace std;

Woodcutter::Woodcutter(){
    cout << "create " << WOODCUTTER << endl;
}

Woodcutter::~Woodcutter(){
    cout << "delete " << WOODCUTTER << endl;
}

// +1購入、+2コイン
ActionDone Woodcutter::action(PLAYERCARD_CONCRETECARD_IO *p){
    cout << "action " << WOODCUTTER << endl;
    p->resultData.data.buy = 1;
    p->resultData.data.coin = 2;
    return ACTIONDONE;
}

const string* Woodcutter::who(){
    return &WOODCUTTER;
}
