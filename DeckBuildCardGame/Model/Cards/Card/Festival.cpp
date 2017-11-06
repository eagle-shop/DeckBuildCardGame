#include <iostream>
#include "General.hpp"
#include "Festival.hpp"
#include "PlayerCards.hpp"

using namespace std;

Festival::Festival(){
    cout << "create " << FESTIVAL << endl;
}

Festival::~Festival(){
    cout << "delete " << FESTIVAL << endl;
}

// +2アクション、+1購入、+2コイン
ActionDone Festival::action(PLAYERCARD_CONCRETECARD_IO *p){
    cout << "action " << FESTIVAL << endl;
    p->resultData.data.action = 2;
    p->resultData.data.buy = 1;
    p->resultData.data.coin = 2;
    return ACTIONDONE;
}

const string* Festival::who(){
    return &FESTIVAL;
}
