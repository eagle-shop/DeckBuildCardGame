#include <iostream>
#include "General.hpp"
#include "Smithy.hpp"
#include "PlayerCards.hpp"

using namespace std;

Smithy::Smithy(){
    cout << "create " << SMITHY << endl;
}

Smithy::~Smithy(){
    cout << "delete " << SMITHY << endl;
}

// +3カード
ActionDone Smithy::action(PLAYERCARD_CONCRETECARD_IO *p){
    cout << "action " << SMITHY << endl;
    p->resultData.drawCard = 3;
    return ACTIONDONE;
}

const string* Smithy::who(){
    return &SMITHY;
}
