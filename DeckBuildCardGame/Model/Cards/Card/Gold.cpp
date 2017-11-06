#include <iostream>
#include "General.hpp"
#include "Gold.hpp"

using namespace std;

Gold::Gold(){
    cout << "create " << GOLD << endl;
}

Gold::~Gold(){
    cout << "delete " << GOLD << endl;
}

ActionDone Gold::action(PLAYERCARD_CONCRETECARD_IO *p){
    cout << "action " << GOLD << endl;
    return ACTIONUNDONE;
}

const string* Gold::who(){
    return &GOLD;
}
