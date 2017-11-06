#include <iostream>
#include "General.hpp"
#include "Laboratory.hpp"
#include "PlayerCards.hpp"

using namespace std;

Laboratory::Laboratory(){
    cout << "create " << LABOTATORY << endl;
}

Laboratory::~Laboratory(){
    cout << "delete " << LABOTATORY << endl;
}

// +2カード、+1アクション
ActionDone Laboratory::action(PLAYERCARD_CONCRETECARD_IO *p){
    cout << "action " << LABOTATORY << endl;
    p->resultData.drawCard = 2;
    p->resultData.data.action = 1;
    return ACTIONDONE;
}

const string* Laboratory::who(){
    return &LABOTATORY;
}
