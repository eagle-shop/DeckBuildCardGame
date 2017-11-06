#include <iostream>
#include "General.hpp"
#include "Curse.hpp"

using namespace std;

Curse::Curse(){
    cout << "create " << CURSE << endl;
}

Curse::~Curse(){
    cout << "delete " << CURSE << endl;
}

ActionDone Curse::action(PLAYERCARD_CONCRETECARD_IO *p){
    cout << "action " << CURSE << endl;
    return ACTIONUNDONE;
}

const string* Curse::who(){
    return &CURSE;
}
