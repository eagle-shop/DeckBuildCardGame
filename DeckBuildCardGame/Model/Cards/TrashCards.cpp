#include <iostream>
#include "TrashCards.hpp"
#include "EventCtoM.hpp"
#include "P2P_IF.hpp"

using namespace std;

// 実装中・・・

TrashCards::TrashCards(){
    cout << "create TrashCards" << endl;
}

TrashCards::~TrashCards(){
    cout << "delete TrashCards" << endl;
}

void TrashCards::addCard(const string *name){
    m_trashCards.push_back(name);
    // P2P通知
    EVENT_CtoM tmp = {};
    P2P_createData(&tmp, EVENT_P2P_RECEIVEDATA, PHASE_UNKNOWN, &TRASHAREA, name);
    P2P_sendData(&tmp, sizeof(tmp));
}

void TrashCards::addCardOtherPlayer(const string *name){
    m_trashCards.push_back(name);
}

void TrashCards::createData(vector<const string*> *name){
    *name = m_trashCards;
}
