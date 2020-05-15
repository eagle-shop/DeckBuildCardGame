#include <iostream>
#include <random>
#include "Model.hpp"
#include "AllCard.hpp"
#include "PlayerCards.hpp"
#include "P2P_IF.hpp"

using namespace std;

PlayerCards::PlayerCards(){
    cout << "create PlayerCards" << endl;
    // 初期の銅貨7枚、屋敷3枚を生成して捨札に追加
    for(auto i = 0; i < 7; i++){
        m_discardCards.push_back(new Copper());
    }
    for(auto i = 0; i < 3; i++){
        m_discardCards.push_back(new Estate());
    }
    // 捨札をシャッフルして山札に
    shuffleDiscardToDeck();
    // 山札から5枚を手札に
    moveCards(&m_handCards.handCards, &m_deckCards, 5, BACK);
}

PlayerCards::~PlayerCards(){
    cout << "delete PlayerCards" << endl;

    for(auto& handCard : m_handCards.handCards){
        delete handCard;
    }
    for(auto& discardCard : m_discardCards){
        delete discardCard;
    }
    for(auto& deckCard : m_deckCards){
        delete deckCard;
    }
    for(auto&playCard : m_playCards){
        delete playCard;
    }
    for(auto& tmpCard : m_tmpCards){
        delete tmpCard;
    }
}

void PlayerCards::createData(PLAYERCARDS *playerCards){
    // 通知データ生成
    playerCards->deckCardNum = m_deckCards.size();

    for(auto handCard : m_handCards.handCards){
        // カード名をコピー
        playerCards->handCards.handCardsName.push_back(handCard->who());
        // 表示状態をコピー ????
        playerCards->handCards.drawState.push_back(DRAW_NOMAL);
    }
    for(auto playCard : m_playCards){
        // カード名をコピー
        playerCards->playCardsName.push_back(playCard->who());
    }
    // ????
    if(m_handCards.drawState.size() > 0){
        playerCards->handCards.drawState = m_handCards.drawState;
        m_handCards.drawState.clear();
    }
}

// プレイヤーカード全出力
void PlayerCards::allPrint(){
    cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
    for(auto i = 0; i < m_deckCards.size(); i++){
        cout << "[m_deckCards " << i << "] " << *(m_deckCards[i]->who()) << endl;
    }
    for(auto i = 0; i < m_handCards.handCards.size(); i++){
        cout << "[m_handCards " << i << "] " << *(m_handCards.handCards[i]->who()) << endl;
    }
    for(auto i = 0; i < m_discardCards.size(); i++){
        cout << "[m_discardCards " << i << "] " << *(m_discardCards[i]->who()) << endl;
    }
    for(auto i = 0; i < m_playCards.size(); i++){
        cout << "[m_playCards " << i << "] " << *(m_playCards[i]->who()) << endl;
    }
    for(auto i = 0; i < m_tmpCards.size(); i++){
        cout << "[m_tmpCards " << i << "] " << *(m_tmpCards[i]->who()) << endl;
    }
    cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
}

// カード一枚購入して捨札に追加
void PlayerCards::buy(const string *name){
    if(*name == COPPER){
        m_discardCards.push_back(new Copper());
    }
    else if(*name == SILVER){
        m_discardCards.push_back(new Silver());
    }
    else if(*name == GOLD){
        m_discardCards.push_back(new Gold());
    }
    else if(*name == ESTATE){
        m_discardCards.push_back(new Estate());
    }
    else if(*name == DUCHY){
        m_discardCards.push_back(new Duchy());
    }
    else if(*name == PROVINCE){
        m_discardCards.push_back(new Province());
    }
    else if(*name == CURSE){
        m_discardCards.push_back(new Curse());
    }
    else if(*name == CHAPEL){
        m_discardCards.push_back(new Chapel());
    }
    else if(*name == CELLAR){
        m_discardCards.push_back(new Cellar());
    }
    else if(*name == WOODCUTTER){
        m_discardCards.push_back(new Woodcutter());
    }
    else if(*name == SMITHY){
        m_discardCards.push_back(new Smithy());
    }
    else if(*name == MARKET){
        m_discardCards.push_back(new Market());
    }
    else if(*name == LABOTATORY){
        m_discardCards.push_back(new Laboratory());
    }
    else if(*name == FESTIVAL){
        m_discardCards.push_back(new Festival());
    }
    else if(*name == ADVENTURER){
        m_discardCards.push_back(new Adventurer());
    }
}

// クリーンアップ
void PlayerCards::cleanup(){
    // 手札から捨札へ
    moveCards(&m_discardCards, &m_handCards.handCards, ALLCARDS, FRONT);
    // プレイエリアから捨札へ
    moveCards(&m_discardCards, &m_playCards, ALLCARDS, FRONT);
    // 山札から手札へ
    moveDeckToHand(5);
}

// 指定した名前のカードが手札にあれば、アクション実施してプレイエリアへ移動
ActionDone PlayerCards::action(MODEL_PLAYERCARD_IO* modelPlayerCard_ioData){
    // パラメータチェック
    if(m_handCards.handCards.size() <= modelPlayerCard_ioData->notifyData.opeIndex){
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
        ret = m_handCards.handCards[modelPlayerCard_ioData->notifyData.opeIndex]->action(&tmp);
        if((ret != ACTIONDONE) && (ret != ACTIONWAIT) && (ret != ACTIONCONTINUE)){
            break;
        }
        else if((ret == ACTIONDONE) || (ret == ACTIONWAIT)){
            modelPlayerCard_ioData->resultData = tmp.resultData.data;
            // 手札からプレイエリアへ
            moveCards(&m_playCards, &m_handCards.handCards, modelPlayerCard_ioData->notifyData.opeIndex);
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
                if((m_deckCards.size() > 0) || (m_discardCards.size() > 0)){
                    if(m_deckCards.size() <= 0){
                        // 捨札をシャッフルして山札に
                        shuffleDiscardToDeck();
                    }
                    tmp.notifyData.data.opeElement = m_deckCards[m_deckCards.size() - 1]->who();
                    tmp.notifyData.data.opeIndex = i;
                    moveCards(&m_tmpCards, &m_deckCards, m_deckCards.size() - 1);
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
        for(auto cardIndex : tmp.resultData.data.cardIndex){
            moveCards(&m_handCards.handCards, &m_tmpCards, cardIndex);
        }
    }
    // 一時置き場にあるカードを捨札に移動
    if(m_tmpCards.size() > 0){
        moveCards(&m_discardCards, &m_tmpCards, ALLCARDS, FRONT);
    }
    return ret;
}

// アクションがユーザ待ちの場合は、操作内容を通知する
ActionDone PlayerCards::actionForWaitAction(MODEL_PLAYERCARD_IO* modelPlayerCard_ioData){
    // パラメータチェック
    if(m_playCards.size() == 0){
        return ACTIONUNDONE;
    }
    // 通知データ生成
    PLAYERCARD_CONCRETECARD_IO tmp = {};
    {
        tmp.notifyData.actionPhase = PHASE_ACTIONWAIT;
        tmp.notifyData.data = modelPlayerCard_ioData->notifyData;
    }
    // アクション実施
    auto ret = m_playCards[m_playCards.size() - 1]->action(&tmp);
    modelPlayerCard_ioData->resultData = tmp.resultData.data;
    // 手札を選択状態にする
    if(tmp.resultData.data.ope == SELECTHANDCARD){
        m_handCards.drawState.clear();
        // まず全部通常状態で初期化
        for(auto i = 0; i < m_handCards.handCards.size(); i++){
            m_handCards.drawState.push_back(DRAW_NOMAL);
        }
        // 指定の番号だけ選択状態に変更
        for(auto cardIndex : tmp.resultData.data.cardIndex){
            m_handCards.drawState[cardIndex] = DRAW_SELECT;
        }
    }
    // 選択した手札を捨札に移動する
    else if(tmp.resultData.data.ope == GOTODISCARD){
        // データを降順にソート
        sort(tmp.resultData.data.cardIndex.begin(), tmp.resultData.data.cardIndex.end(), greater<int>());
        for(auto cardIndex : tmp.resultData.data.cardIndex){
            moveCards(&m_discardCards, &m_handCards.handCards, cardIndex);
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
    const string *name = m_handCards.handCards[cardindex]->who();
    // カード情報参照
    CardInfo &cardInfo = CardInfo::getInstance();
    // 財宝カードの場合
    if(cardInfo._cardInfo[*name].effectInfo.kind == KIND_TREASURE){
        // コインを取得
        auto ret = cardInfo._cardInfo[*name].effectInfo.coin;
        // 手札からプレイエリアへ
        moveCards(&m_playCards, &m_handCards.handCards, cardindex);
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
        if(cardIndex >= m_handCards.handCards.size()){
            return NULL;
        }
        auto tmp = m_handCards.handCards[cardIndex];
        auto name = tmp->who();
        m_handCards.handCards.erase(m_handCards.handCards.begin() + cardIndex);
        delete tmp;
        return name;
    }
    return NULL;
}

// 捨札をシャッフルして山札に移動
bool PlayerCards::shuffleDiscardToDeck(){
    // 山札にカードがあればエラー
    if(m_deckCards.size() > 0){
        return false;
    }
    // ランダム変数生成
    random_device rd;
    mt19937_64 mt(rd());
    // 捨札シャッフル
    shuffle(m_discardCards.begin(), m_discardCards.end(), mt);
    // 捨札 -> 山札　全移動
    moveCards(&m_deckCards, &m_discardCards, ALLCARDS, FRONT);

    return true;
}

// 山札から手札に移動
void PlayerCards::moveDeckToHand(CARDNUM cardNum){
    // 山札から手札へ
    auto handNum = moveCards(&m_handCards.handCards, &m_deckCards, cardNum, BACK);
    // 手札に指定枚数なければ捨札から山札へ
    if(handNum != 0){
        // 捨札をシャッフルして山札に
        shuffleDiscardToDeck();
        // 山札から手札へ
        moveCards(&m_handCards.handCards, &m_deckCards, handNum, BACK);
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
    m_playCardsOtherPlayer.push_back(name);
}

void PlayerCards::clearPlayCardOtherPlayer(){
    m_playCardsOtherPlayer.clear();
}

void PlayerCards::createDataOtherPlayerCards(PLAYERCARDS *playerCards){
    // 通知データ生成
    playerCards->deckCardNum = m_deckCards.size();
    for(auto handCards : m_handCards.handCards){
        // カード名をコピー
        playerCards->handCards.handCardsName.push_back(handCards->who());
        // 表示状態をコピー ????
        playerCards->handCards.drawState.push_back(DRAW_NOMAL);
    }
    // カード名をコピー
    playerCards->playCardsName = m_playCardsOtherPlayer;// ここだけ変えてみた
    // ????
    if(m_handCards.drawState.size() > 0){
        playerCards->handCards.drawState = m_handCards.drawState;
        m_handCards.drawState.clear();
    }
}
