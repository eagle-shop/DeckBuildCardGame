#include <iostream>
#include "General.hpp"
#include "Chapel.hpp"
#include "PlayerCards.hpp"

using namespace std;

Chapel::Chapel(){
    cout << "create " << CHAPEL << endl;
}

Chapel::~Chapel(){
    cout << "delete " << CHAPEL << endl;
}

// 手札から最大4枚まで選択して廃棄
ActionDone Chapel::action(PLAYERCARD_CONCRETECARD_IO *p){
    cout << "action " << CHAPEL << endl;

    // 初めてプレイエリアに出した時はアクション待ちを返す
    if(p->notifyData.actionPhase == PHASE_ACTION){
        _select.clear();
        return ACTIONWAIT;
    }

    // 完了SWの場合
    if(STR_SEARCH(p->notifyData.data.opeElement, OTHERSWCOMPLETE)){
        p->resultData.data.ope = GOTOTRASH;
        p->resultData.data.cardIndex = _select;
        return ACTIONDONE;
    }
    // キャンセルSWの場合
    else if(STR_SEARCH(p->notifyData.data.opeElement, OTHERSWCHANSEL)){
        _select.clear();
        p->resultData.data.ope = SELECTHANDCARD;
        p->resultData.data.cardIndex.clear();
        return ACTIONWAIT;
    }
    // 手札のカード
    // 選択済みが4枚未満
    // 未選択のカード
    if((STR_SEARCH(p->notifyData.data.area, HAND))
       && (_select.size() < 4)
       && (_select.end() == find(_select.begin(), _select.end(), p->notifyData.data.opeIndex))){
        // 選択したカードを保持
        _select.push_back(p->notifyData.data.opeIndex);
    }
    p->resultData.data.ope = SELECTHANDCARD;
    p->resultData.data.cardIndex = _select;
    return ACTIONWAIT;
}

const string* Chapel::who(){
    return &CHAPEL;
}
