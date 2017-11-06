#include <iostream>
#include "General.hpp"
#include "Silver.hpp"

using namespace std;

Silver::Silver(){
    cout << "create " << SILVER << endl;
}

Silver::~Silver(){
    cout << "delete " << SILVER << endl;
}

ActionDone Silver::action(PLAYERCARD_CONCRETECARD_IO *p){
    cout << "action " << SILVER << endl;
    return ACTIONUNDONE;
}

const string* Silver::who(){
    return &SILVER;
}
