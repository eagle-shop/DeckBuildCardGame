#include <iostream>
#include "General.hpp"
#include "Adventurer.hpp"
#include "PlayerCards.hpp"

using namespace std;

Adventurer::Adventurer(){
    cout << "create " << ADVENTURER << endl;
}

Adventurer::~Adventurer(){
    cout << "delete " << ADVENTURER << endl;
}

// 山札を上から1枚ずつ公開して、財宝カード2枚出たら手札に加えて終了
ActionDone Adventurer::action(PLAYERCARD_CONCRETECARD_IO *p){
    cout << "action " << ADVENTURER << endl;
    // 初回コール
    if(p->notifyData.actionPhase == PHASE_ACTION){
        p->resultData.data.ope = DECKOPEN;
        m_treasure.clear();
        return ACTIONCONTINUE;
    }
    // 2回目以降（山札1枚公開毎にコール）
    else if((p->notifyData.actionPhase == PHASE_UNKNOWN) && (*(p->notifyData.data.opeElement) != STRNOTHING)){
        // カード情報参照
        CardInfo &cardInfo = CardInfo::getInstance();
        // 財宝カードの場合
        if(cardInfo._cardInfo[*(p->notifyData.data.opeElement)].effectInfo.kind == KIND_TREASURE){
            m_treasure.push_back(p->notifyData.data.opeIndex);
        }
        // 2枚未満なら継続
        if(m_treasure.size() < 2){
            p->resultData.data.ope = DECKOPEN;
            return ACTIONCONTINUE;
        }
    }
    // 保持した財宝カードを手札に加える
    p->resultData.data.ope = GOTOHANDCARD;
    p->resultData.data.cardIndex = m_treasure;
    return ACTIONDONE;
}

const string* Adventurer::who(){
    return &ADVENTURER;
}
