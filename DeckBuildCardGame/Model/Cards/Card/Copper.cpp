#include <iostream>
#include "General.hpp"
#include "Copper.hpp"

using namespace std;

Copper::Copper(){
    cout << "create " << COPPER << endl;
}

Copper::~Copper(){
    cout << "delete " << COPPER << endl;
}

ActionDone Copper::action(PLAYERCARD_CONCRETECARD_IO *p){
    cout << "action " << COPPER << endl;
    return ACTIONUNDONE;
}

const string* Copper::who(){
    return &COPPER;
}
