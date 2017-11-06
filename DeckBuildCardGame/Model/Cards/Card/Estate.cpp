#include <iostream>
#include "General.hpp"
#include "Estate.hpp"

using namespace std;

Estate::Estate(){
    cout << "create " << ESTATE << endl;
}

Estate::~Estate(){
    cout << "delete " << ESTATE << endl;
}

ActionDone Estate::action(PLAYERCARD_CONCRETECARD_IO *p){
    cout << "action " << ESTATE << endl;
    return ACTIONUNDONE;
}

const string* Estate::who(){
    return &ESTATE;
}
