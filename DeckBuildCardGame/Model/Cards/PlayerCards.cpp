#include <iostream>
#include <random>
#include "Model.hpp"
#include "AllCard.hpp"
#include "PlayerCards.hpp"
#include "P2P_IF.hpp"

using namespace std;

PlayerCards::PlayerCards(){
    cout << "create PlayerCards" << endl;
}

PlayerCards::~PlayerCards(){
    cout << "delete PlayerCards" << endl;

    for(auto i = 0; i < _handCards.handCards.size(); i++){
        delete _handCards.handCards[i];
    }
    for(auto i = 0; i < _discardCards.size(); i++){
        delete _discardCards[i];
    }
    for(auto i = 0; i < _deckCards.size(); i++){
        delete _deckCards[i];
    }
    for(auto i = 0; i < _playCards.size(); i++){
        delete _playCards[i];
    }
    for(auto i = 0; i < _tmpCards.size(); i++){
        delete _tmpCards[i];
    }
}

// 初期化（とりあえず屋敷スタート）
void PlayerCards::init(){
    // 初期の銅貨7枚、屋敷3枚を生成して捨札に追加
    for(auto i = 0; i < 7; i++){
        _discardCards.push_back(new Copper());
    }
    for(auto i = 0; i < 3; i++){
        _discardCards.push_back(new Estate());
    }
    // 捨札をシャッフルして山札に
    shuffleDiscardToDeck();
    // 山札から5枚を手札に
    moveCards(&_handCards.handCards, &_deckCards, 5, BACK);
}

void PlayerCards::createData(PLAYERCARDS *playerCards){
    // 通知データ生成
    playerCards->deckCardNum = _deckCards.size();
    for(auto i = 0; i < _handCards.handCards.size(); i++){
        // カード名をコピー
        playerCards->handCards.handCardsName.push_back(_handCards.handCards[i]->who());
        // 表示状態をコピー ????
        playerCards->handCards.drawState.push_back(DRAW_NOMAL);
    }
    for(auto i = 0; i < _playCards.size(); i++){
        // カード名をコピー
        playerCards->playCardsName.push_back(_playCards[i]->who());
    }
    // ????
    if(_handCards.drawState.size() > 0){
        playerCards->handCards.drawState = _handCards.drawState;
        _handCards.drawState.clear();
    }
}

// プレイヤーカード全出力
void PlayerCards::allPrint(){
    cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
    for(auto i = 0; i < _deckCards.size(); i++){
        cout << "[_deckCards " << i << "] " << *(_deckCards[i]->who()) << endl;
    }
    for(auto i = 0; i < _handCards.handCards.size(); i++){
        cout << "[_handCards " << i << "] " << *(_handCards.handCards[i]->who()) << endl;
    }
    for(auto i = 0; i < _discardCards.size(); i++){
        cout << "[_discardCards " << i << "] " << *(_discardCards[i]->who()) << endl;
    }
    for(auto i = 0; i < _playCards.size(); i++){
        cout << "[_playCards " << i << "] " << *(_playCards[i]->who()) << endl;
    }
    for(auto i = 0; i < _tmpCards.size(); i++){
        cout << "[_tmpCards " << i << "] " << *(_tmpCards[i]->who()) << endl;
    }
    cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
}

// カード一枚購入して捨札に追加
void PlayerCards::buy(const string *name){
    if(*name == COPPER){
        _discardCards.push_back(new Copper());
    }
    else if(*name == SILVER){
        _discardCards.push_back(new Silver());
    }
    else if(*name == GOLD){
        _discardCards.push_back(new Gold());
    }
    else if(*name == ESTATE){
        _discardCards.push_back(new Estate);
    }
    else if(*name == DUCHY){
        _discardCards.push_back(new Duchy);
    }
    else if(*name == PROVINCE){
        _discardCards.push_back(new Province);
    }
    else if(*name == CURSE){
        _discardCards.push_back(new Curse);
    }
    else if(*name == CHAPEL){
        _discardCards.push_back(new Chapel());
    }
    else if(*name == CELLAR){
        _discardCards.push_back(new Cellar());
    }
    else if(*name == WOODCUTTER){
        _discardCards.push_back(new Woodcutter());
    }
    else if(*name == SMITHY){
        _discardCards.push_back(new Smithy());
    }
    else if(*name == MARKET){
        _discardCards.push_back(new Market());
    }
    else if(*name == LABOTATORY){
        _discardCards.push_back(new Laboratory());
    }
    else if(*name == FESTIVAL){
        _discardCards.push_back(new Festival());
    }
    else if(*name == ADVENTURER){
        _discardCards.push_back(new Adventurer());
    }
}

// クリーンアップ
void PlayerCards::cleanup(){
    // 手札から捨札へ
    moveCards(&_discardCards, &_handCards.handCards, ALLCARDS, FRONT);
    // プレイエリアから捨札へ
    moveCards(&_discardCards, &_playCards, ALLCARDS, FRONT);
    // 山札から手札へ
    moveDeckToHand(5);
}

// 指定した名前のカードが手札にあれば、アクション実施してプレイエリアへ移動
ActionDone PlayerCards::action(MODEL_PLAYERCARD_IO* modelPlayerCard_ioData){
    // パラメータチェック
    if(_handCards.handCards.size() <= modelPlayerCard_ioData->notifyData.opeIndex){
        return ACTIONUNDONE;
    }
    // 通知データ生成
    PLAYERCARD_CONCRETECARD_IO tmp = {};
    tmp.notifyData.actionPhase = PHASE_ACTION;
    tmp.notifyData.data = modelPlayerCard_ioData->notifyData;
    // アクション実施
    ActionDone ret;
    auto i = 0;
    do{
        allPrint();
        ret = _handCards.handCards[modelPlayerCard_ioData->notifyData.opeIndex]->action(&tmp);
        if((ret != ACTIONDONE) && (ret != ACTIONWAIT) && (ret != ACTIONCONTINUE)){
            break;
        }
        else if((ret == ACTIONDONE) || (ret == ACTIONWAIT)){
            modelPlayerCard_ioData->resultData = tmp.resultData.data;
            // 手札からプレイエリアへ
            moveCards(&_playCards, &_handCards.handCards, modelPlayerCard_ioData->notifyData.opeIndex);
            // P2P通知
            EVENT_CtoM tmpData = {};
            P2P_createData(&tmpData, EVENT_P2P_RECEIVEDATA, PHASE_UNKNOWN, &PLAYAREA, modelPlayerCard_ioData->notifyData.opeElement);
            P2P_sendData(&tmpData, sizeof(tmpData));
        }
        // 暫定：追加情報が欲しい時（？）とりあえずは冒険者用
        else if(ret == ACTIONCONTINUE){
            // 山札公開（？）
            if(tmp.resultData.data.ope == DECKOPEN){
                tmp = {};
                tmp.notifyData.actionPhase = PHASE_UNKNOWN;
                // 山札か捨札があるとき
                if((_deckCards.size() > 0) || (_discardCards.size() > 0)){
                    if(_deckCards.size() <= 0){
                        // 捨札をシャッフルして山札に
                        shuffleDiscardToDeck();
                    }
                    tmp.notifyData.data.opeElement = _deckCards[_deckCards.size() - 1]->who();
                    tmp.notifyData.data.opeIndex = i;
                    moveCards(&_tmpCards, &_deckCards, _deckCards.size() - 1);
                }
                // 山札も捨札も無いとき
                else{
                    tmp.notifyData.data.opeElement = &STRNOTHING;
                }
            }
        }
        i++;
    }while(ret == ACTIONCONTINUE);

    // カード移動がある場合
    if(tmp.resultData.drawCard > 0){
        moveDeckToHand(tmp.resultData.drawCard);
    }
    // 一時置き場にあるカードの中から、指定カードを手札に移動
    if(tmp.resultData.data.ope == GOTOHANDCARD){
        // データを降順にソート
        sort(tmp.resultData.data.cardIndex.begin(), tmp.resultData.data.cardIndex.end(), greater<int>());
        for(auto i = 0; i < tmp.resultData.data.cardIndex.size(); i++){
            moveCards(&_handCards.handCards, &_tmpCards, tmp.resultData.data.cardIndex[i]);
        }
    }
    // 一時置き場にあるカードを捨札に移動
    if(_tmpCards.size() > 0){
        moveCards(&_discardCards, &_tmpCards, ALLCARDS, FRONT);
    }
    return ret;
}

// アクションがユーザ待ちの場合は、操作内容を通知する
ActionDone PlayerCards::actionForWaitAction(MODEL_PLAYERCARD_IO* modelPlayerCard_ioData){
    // パラメータチェック
    if(_playCards.size() == 0){
        return ACTIONUNDONE;
    }
    // 通知データ生成
    PLAYERCARD_CONCRETECARD_IO tmp = {};
    {
        tmp.notifyData.actionPhase = PHASE_ACTIONWAIT;
        tmp.notifyData.data = modelPlayerCard_ioData->notifyData;
    }
    // アクション実施
    auto ret = _playCards[_playCards.size() - 1]->action(&tmp);
    modelPlayerCard_ioData->resultData = tmp.resultData.data;
    // 手札を選択状態にする
    if(tmp.resultData.data.ope == SELECTHANDCARD){
        _handCards.drawState.clear();
        // まず全部通常状態で初期化
        for(auto i = 0; i < _handCards.handCards.size(); i++){
            _handCards.drawState.push_back(DRAW_NOMAL);
        }
        // 指定の番号だけ選択状態に変更
        for(auto i = 0; i < tmp.resultData.data.cardIndex.size(); i++){
            _handCards.drawState[tmp.resultData.data.cardIndex[i]] = DRAW_SELECT;
        }
    }
    // 選択した手札を捨札に移動する
    else if(tmp.resultData.data.ope == GOTODISCARD){
        // データを降順にソート
        sort(tmp.resultData.data.cardIndex.begin(), tmp.resultData.data.cardIndex.end(), greater<int>());
        for(auto i = 0; i < tmp.resultData.data.cardIndex.size(); i++){
            moveCards(&_discardCards, &_handCards.handCards, tmp.resultData.data.cardIndex[i]);
        }
    }
    // カード移動がある場合
    if(tmp.resultData.drawCard > 0){
        moveDeckToHand(tmp.resultData.drawCard);
    }

    return ret;
}

// 指定した名前のカードが手札にあり、財宝カードならプレイエリアに移動してコイン数を返す
COINNUM PlayerCards::treasureMoveToPlayArea(CARDINDEX cardindex){
    const string *name = _handCards.handCards[cardindex]->who();
    // カード情報参照
    CardInfo &cardInfo = CardInfo::getInstance();
    // 財宝カードの場合
    if(cardInfo._cardInfo[*name].effectInfo.kind == KIND_TREASURE){
        // コインを取得
        auto ret = cardInfo._cardInfo[*name].effectInfo.coin;
        // 手札からプレイエリアへ
        moveCards(&_playCards, &_handCards.handCards, cardindex);
        // P2P通知
        EVENT_CtoM tmpData = {};
        P2P_createData(&tmpData, EVENT_P2P_RECEIVEDATA, PHASE_UNKNOWN, &PLAYAREA, name);
        P2P_sendData(&tmpData, sizeof(tmpData));
        return ret;
    }
    return 0;
}

// カード削除（連続操作は降順でやること）
const string* PlayerCards::removeCard(AREA* area, CARDINDEX cardIndex){
    // 手札
    if(STR_SEARCH(area, HAND)){
        if(cardIndex >= _handCards.handCards.size()){
            return NULL;
        }
        auto tmp = _handCards.handCards[cardIndex];
        auto name = tmp->who();
        _handCards.handCards.erase(_handCards.handCards.begin() + cardIndex);
        delete tmp;
        return name;
    }
    return NULL;
}

// 捨札をシャッフルして山札に移動
bool PlayerCards::shuffleDiscardToDeck(){
    // 山札にカードがあればエラー
    if(_deckCards.size() > 0){
        return false;
    }
    // ランダム変数生成
    random_device rd;
    mt19937_64 mt(rd());
    // 捨札シャッフル
    shuffle(_discardCards.begin(), _discardCards.end(), mt);
    // 捨札 -> 山札　全移動
    moveCards(&_deckCards, &_discardCards, ALLCARDS, FRONT);

    return true;
}

// 山札から手札に移動
void PlayerCards::moveDeckToHand(CARDNUM cardNum){
    // 山札から手札へ
    auto handNum = moveCards(&_handCards.handCards, &_deckCards, cardNum, BACK);
    // 手札に指定枚数なければ捨札から山札へ
    if(handNum != 0){
        // 捨札をシャッフルして山札に
        shuffleDiscardToDeck();
        // 山札から手札へ
        moveCards(&_handCards.handCards, &_deckCards, handNum, BACK);
    }
}

// 指定枚数をまとめてカード移動
CARDNUM PlayerCards::moveCards(vector<Card*> *dst, vector<Card*> *src, CARDNUM moveNum, Direction direction){
    // 実際に移動できる枚数を決定する
    auto realMoveNum = moveNum;
    // 移動元に指定枚数のカードがないときは、移動元枚数に合わせる
    if(moveNum > src->size()){
        realMoveNum = src->size();
    }
    // 移動元の位置を決定する
    auto istart = src->begin();
    auto iend = src->begin() + realMoveNum;
    // 逆順なら位置を後ろからにする
    if(direction == BACK){
        istart = src->end() - realMoveNum;
        iend = src->end();
    }
    // コピー
    copy(istart, iend, back_inserter(*dst));
    // 移動元削除
    src->erase(istart, iend);

    return (moveNum - realMoveNum);
}

// カード1枚移動（連続操作は降順でやること）
bool PlayerCards::moveCards(vector<Card*> *dst, vector<Card*> *src, CARDINDEX index){
    // 移動元カードが存在しないとき
    if(src->size() <= index){
        return false;
    }
    // 移動先に追加
    dst->push_back((*src)[index]);
    // 移動元削除
    src->erase(src->begin() + index);
    return true;
}

void PlayerCards::addPlayCardOtherPlayer(const string *name){
    _playCardsOtherPlayer.push_back(name);
}

void PlayerCards::clearPlayCardOtherPlayer(){
    _playCardsOtherPlayer.clear();
}

void PlayerCards::createDataOtherPlayerCards(PLAYERCARDS *playerCards){
    // 通知データ生成
    playerCards->deckCardNum = _deckCards.size();
    for(auto i = 0; i < _handCards.handCards.size(); i++){
        // カード名をコピー
        playerCards->handCards.handCardsName.push_back(_handCards.handCards[i]->who());
        // 表示状態をコピー ????
        playerCards->handCards.drawState.push_back(DRAW_NOMAL);
    }
    // カード名をコピー
    playerCards->playCardsName = _playCardsOtherPlayer;// ここだけ変えてみた
    // ????
    if(_handCards.drawState.size() > 0){
        playerCards->handCards.drawState = _handCards.drawState;
        _handCards.drawState.clear();
    }
}
