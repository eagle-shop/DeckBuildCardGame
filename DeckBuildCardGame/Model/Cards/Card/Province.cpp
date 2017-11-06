#include <iostream>
#include "General.hpp"
#include "Province.hpp"

using namespace std;

Province::Province(){
    cout << "create " << PROVINCE << endl;
}

Province::~Province(){
    cout << "delete " << PROVINCE << endl;
}

ActionDone Province::action(PLAYERCARD_CONCRETECARD_IO *p){
    cout << "action " << PROVINCE << endl;
    return ACTIONUNDONE;
}

const string* Province::who(){
    return &PROVINCE;
}
