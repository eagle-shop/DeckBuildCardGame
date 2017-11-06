#include <iostream>
#include "General.hpp"
#include "Duchy.hpp"

using namespace std;

Duchy::Duchy(){
    cout << "create " << DUCHY << endl;
}

Duchy::~Duchy(){
    cout << "delete " << DUCHY << endl;
}

ActionDone Duchy::action(PLAYERCARD_CONCRETECARD_IO *p){
    cout << "action " << DUCHY << endl;
    return ACTIONUNDONE;
}

const string* Duchy::who(){
    return &DUCHY;
}
