#include <iostream>
#include "General.hpp"
#include "SupplyCards.hpp"
#include "EventCtoM.hpp"
#include "P2P_IF.hpp"

using namespace std;

SupplyCards::SupplyCards(){
    cout << "create SupplyCards" << endl;
}

SupplyCards::~SupplyCards(){
    cout << "delete SupplyCards" << endl;
}

// サプライの初期化
void SupplyCards::init(){
    m_supplyCards.push_back({&COPPER, 60 - (7 * 2)});   // 60枚から手札を引いた数が初期値
    m_supplyCards.push_back({&CHAPEL, 12});
    m_supplyCards.push_back({&CELLAR, 12});
    m_supplyCards.push_back({&WOODCUTTER, 12});
    m_supplyCards.push_back({&SMITHY, 12});
    m_supplyCards.push_back({&MARKET, 12});
    m_supplyCards.push_back({&SILVER, 40});
    m_supplyCards.push_back({&LABOTATORY, 12});
    m_supplyCards.push_back({&FESTIVAL, 12});
    m_supplyCards.push_back({&ADVENTURER, 12});
    m_supplyCards.push_back({&FESTIVAL, 12});
    m_supplyCards.push_back({&FESTIVAL, 12});
    m_supplyCards.push_back({&GOLD, 30});
    m_supplyCards.push_back({&ESTATE, 8});
    m_supplyCards.push_back({&DUCHY, 8});
    m_supplyCards.push_back({&PROVINCE, 8});
    m_supplyCards.push_back({&CURSE, 10});
}

void SupplyCards::createData(vector<SUPPLYCARDS> *supplyCards){
    // 通知データにコピー
    *supplyCards = m_supplyCards;
}

// 指定したカードを購入、残りコインを返す
COINRELATIVE SupplyCards::buy(const string *name, COINNUM coin){
    for(auto& supplyCards : m_supplyCards){
        if(*(supplyCards.cardName) == *name){
            CardInfo &cardInfo = CardInfo::getInstance();
            if((supplyCards.num > 0) && (cardInfo._cardInfo[*name].effectInfo.cost <= coin)){
                supplyCards.num--;
                // P2P通知
                EVENT_CtoM tmp = {};
                P2P_createData(&tmp, EVENT_P2P_RECEIVEDATA, PHASE_UNKNOWN, &SUPPLYAREA, name);
                P2P_sendData(&tmp, sizeof(tmp));

                return coin - cardInfo._cardInfo[*name].effectInfo.cost;
            }
            else{
                return (-99);
            }
        }
    }
    return (-99);
}

void SupplyCards::buyOtherPlayer(const string *name){
    for(auto& supplyCards : m_supplyCards){
        if(*(supplyCards.cardName) == *name){
            if(supplyCards.num > 0){
                supplyCards.num--;
            }
            else{
                // nop
            }
        }
    }
}
