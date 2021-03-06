#include <iostream>
#include "General.hpp"
#include "Cellar.hpp"
#include "PlayerCards.hpp"

using namespace std;

Cellar::Cellar(){
    cout << "create " << CELLAR << endl;
}

Cellar::~Cellar(){
    cout << "delete " << CELLAR << endl;
}

// 手札から捨札にするカードを好きな枚数選び、その分山札から手札に加える
ActionDone Cellar::action(PLAYERCARD_CONCRETECARD_IO *p){
    cout << "action " << CELLAR << endl;

    // 初めてプレイエリアに出した時はアクション待ちを返す
    if(p->notifyData.actionPhase == PHASE_ACTION){
        m_select.clear();
        return ACTIONWAIT;
    }

    // 完了SWの場合
    if(STR_SEARCH(p->notifyData.data.opeElement, OTHERSWCOMPLETE)){
        p->resultData.data.ope = GOTODISCARD;
        p->resultData.data.action = 1;
        p->resultData.data.cardIndex = m_select;
        p->resultData.drawCard = m_select.size();
        return ACTIONDONE;
    }
    // キャンセルSWの場合
    else if(STR_SEARCH(p->notifyData.data.opeElement, OTHERSWCHANSEL)){
        m_select.clear();
        p->resultData.data.ope = SELECTHANDCARD;
        p->resultData.data.cardIndex.clear();
        return ACTIONWAIT;
    }
    // 手札のカード
    // 選択済みが4枚未満
    // 未選択のカード
    if((STR_SEARCH(p->notifyData.data.area, HAND))
       && (m_select.size() < 4)
       && (m_select.end() == find(m_select.begin(), m_select.end(), p->notifyData.data.opeIndex))){
        // 選択したカードを保持
        m_select.push_back(p->notifyData.data.opeIndex);
    }
    p->resultData.data.ope = SELECTHANDCARD;
    p->resultData.data.cardIndex = m_select;
    return ACTIONWAIT;
}

const string* Cellar::who(){
    return &CELLAR;
}
