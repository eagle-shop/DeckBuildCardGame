#include <iostream>
#include <random>
#include "General.hpp"
#include "Model.hpp"

using namespace std;

TIME _time = 0;// 暫定

Model::Model():
#if 1  // リリース
    m_phase(PHASE_OTHERPLAYER),
#else   // デバッグ
    m_phase(PHASE_ACTION),
#endif
    m_actionCount(1),
    m_buyCount(1),
    m_coinCount(0){
    cout << "create Model" << endl;
}

Model::~Model(){
    cout << "delete Model" << endl;
}

void Model::addObserver(Observer_forView *observer){
    m_observer = observer;
}

Phase Model::update(EVENT_CtoM *event){
    // P2P接続時1度のみ受信（再接続は未対応）
    if(event->eventKind == EVENT_P2P_CONNECTED){
        // 先行後攻の判定（とりあえず時間で判断）
        if((_time != 0) && (_time > event->time)){
            m_phase = PHASE_ACTION;
        }
        else if((_time != 0) && (_time < event->time)){
            m_phase = PHASE_OTHERPLAYER;
        }
        else{
            // 異常終了（暫定）
            exit(0);
        }
    }
    // P2P接続先からのデータ受信時
    else if(event->eventKind == EVENT_P2P_RECEIVEDATA){
        // 相手のフェーズが終わった場合
        if((m_phase == PHASE_OTHERPLAYER) && (event->receiveData.phase == PHASE_OTHERPLAYER)){
            m_phase = PHASE_ACTION;
            m_playerCards.clearPlayCardOtherPlayer();
        }
        // 相手のフェーズの場合
        else{
            // receiveDataはchar型は定義済みの文字列参照に変える必要あり
            const string *tmpAreaName = &STRNOTHING;
            const string *tmpCardName = &STRNOTHING;
            {
                const string receiveAreaName = event->receiveData.areaName;
                vector<const string*> tmpAllArea = ALLAREA;
                getString(&tmpAreaName, &receiveAreaName, &tmpAllArea);
                const string receiveCardName = event->receiveData.cardName;
                vector<const string*> tmpAllEle = ALLELE;
                getString(&tmpCardName, &receiveCardName, &tmpAllEle);
            }
            // サプライからカード購入
            if(*tmpAreaName == SUPPLY){
                m_supplyCards.buyOtherPlayer(tmpCardName);
            }
            // カード廃棄
            else if(*tmpAreaName == TRASH){
                m_trashCards.addCardOtherPlayer(tmpCardName);
            }
            // プレイエリアにカード移動
            else if(*tmpAreaName == PLAY){
                m_playerCards.addPlayCardOtherPlayer(tmpCardName);
            }
        }
    }
    // 自分のフェーズ
    else if(event->eventKind == EVENT_SELF){
        switch(m_phase){
            case PHASE_ACTION:
                actionPhase(event);
                break;

            case PHASE_ACTIONWAIT:
                actionWaitPhase(event);
                break;

            case PHASE_BUY:
                buyPhase(event);
                break;

            case PHASE_CLEANUP:
                cleanupPhase();
                m_phase = PHASE_OTHERPLAYER;
#if 0   // デバッグ
                m_phase = PHASE_ACTION;
#endif
                break;

            default:
                break;
        }
    }
    // 描画要求
    draw();
    m_playerCards.allPrint();
    return m_phase;
}

void Model::init(){
    // 描画要求
    draw();
}

void Model::setTime(TIME time){
    _time = time;
}
// アクションフェーズ
void Model::actionPhase(EVENT_CtoM *event){
    // フェーズSW押下
    if(STR_SEARCH(event->self.area, PHASE)){
        // アクション終了の場合
        if(STR_SEARCH(event->self.opeElement, PHASESWENDACTION)){
            m_phase = PHASE_BUY;
        }
        // ターン終了の場合
        else if(STR_SEARCH(event->self.opeElement, PHASESWENDTURN)){
            m_phase = PHASE_CLEANUP;
            update(event);
        }
        return;
    }
    // 手札カード押下orドロップ
    else if(STR_SEARCH(event->self.area, HAND)){
        // プレイエリアにドロップの場合
        if((event->self.TapOrDrag == DRAG)
        && (STR_SEARCH(event->self.dragMoveTo, PLAYAREA))){
            // アクション実行
            auto ret = action(event);

            // アクション完了の場合
            if(ret == ACTIONDONE){
                // アクションが無くなったら購入フェーズへ
                if(m_actionCount <= 0){
                    m_phase = PHASE_BUY;
                }
            }
            // ユーザ操作待ちの場合
            else if(ret == ACTIONWAIT){
                m_phase = PHASE_ACTIONWAIT;
            }
        }
        return;
    }
}

// アクション操作待ちフェーズ
void Model::actionWaitPhase(EVENT_CtoM *event){
    // アクション実行
    auto ret = action(event);
    // アクション完了の場合
    if(ret == ACTIONDONE){
        if(m_actionCount <= 0){
            m_phase = PHASE_BUY;
        }
        else{
            m_phase = PHASE_ACTION;
        }
    }
}

// 購入フェーズ
void Model::buyPhase(EVENT_CtoM *event){
    // フェーズSW押下
    if(STR_SEARCH(event->self.area, PHASE)){
        // 購入終了またはターン終了の場合
        if((STR_SEARCH(event->self.opeElement, PHASESWENDBUY))
        || (STR_SEARCH(event->self.opeElement, PHASESWENDTURN))){
            m_phase = PHASE_CLEANUP;
            update(event);
        }
        return;
    }
    // 手札カード押下orドロップ
    else if(STR_SEARCH(event->self.area, HAND)){
        // プレイエリアにドロップの場合
        if((event->self.TapOrDrag == DRAG)
        && (STR_SEARCH(event->self.dragMoveTo, PLAYAREA))){
            // 財宝カードならコイン追加
            m_coinCount += m_playerCards.treasureMoveToPlayArea(event->self.opeIndex);
        }
        return;
    }
    // サプライ押下
    else if(STR_SEARCH(event->self.area, SUPPLY)){
        // コイン数が0以上ならカード購入成功
        auto coin = m_supplyCards.buy(event->self.opeElement, m_coinCount);
        if(coin >= 0){
            m_coinCount = coin;
            m_buyCount--;
            m_playerCards.buy(event->self.opeElement);
            // 購入数が無くなったら、クリーンアップフェーズへ
            if(m_buyCount <= 0){
                m_phase = PHASE_CLEANUP;
                update(event);
            }
        }
    }
}

// クリーンアップフェーズ
void Model::cleanupPhase(){
    m_playerCards.cleanup();
    m_actionCount = 1;
    m_buyCount = 1;
    m_coinCount = 0;
}

// プレイヤーカードクラスへの通知データを作成
void Model::createActionIoData(EVENT_CtoM *event, MODEL_PLAYERCARD_IO *ioData){
    ioData->notifyData.area = event->self.area;
    ioData->notifyData.opeElement = event->self.opeElement;
    ioData->notifyData.opeIndex = event->self.opeIndex;
    ioData->notifyData.TapOrDrag = event->self.TapOrDrag;
    ioData->notifyData.dragMoveTo = event->self.dragMoveTo;
}

// アクションの実施
ActionDone Model::action(EVENT_CtoM *event){
    ActionDone ret;
    MODEL_PLAYERCARD_IO tmp = {};
    createActionIoData(event, &tmp);
    // カード毎の初回アクション
    if(m_phase == PHASE_ACTION){
        ret = m_playerCards.action(&tmp);
    }
    // アクションでユーザ操作がある場合はこっち
    else if(m_phase == PHASE_ACTIONWAIT){
        ret = m_playerCards.actionForWaitAction(&tmp);
    }
    // アクションかアクション待ちフェーズではないとき
    else{
        ret = ACTIONUNDONE;
    }
    // アクション結果によるクラス間操作
    // 選んだカードを廃棄置き場に移動
    if((tmp.resultData.ope == GOTOTRASH)
       && (tmp.resultData.cardIndex.size() > 0)){
        // データを降順にソート
        sort(tmp.resultData.cardIndex.begin(), tmp.resultData.cardIndex.end(), greater<int>());

        for(auto cardIndex : tmp.resultData.cardIndex){
            // プレイヤーの手札からカードを削除してカード名を取得
            auto name = m_playerCards.removeCard(&HAND, cardIndex);
            // 廃棄置き場に追加
            m_trashCards.addCard(name);
        }
    }
    // acbカウント反映
    if((tmp.resultData.action < 0) && (m_actionCount <= abs(tmp.resultData.action))){
        m_actionCount = 0;
    }
    else{
        m_actionCount += tmp.resultData.action;
    }
    if((tmp.resultData.buy < 0) && (m_buyCount <= abs(tmp.resultData.buy))){
        m_buyCount = 0;
    }
    else{
        m_buyCount += tmp.resultData.buy;
    }
    if((tmp.resultData.coin < 0) && (m_coinCount <= abs(tmp.resultData.coin))){
        m_coinCount = 0;
    }
    else{
        m_coinCount += tmp.resultData.coin;
    }
    if((m_actionCount > 0) && (ret == ACTIONDONE)){
        m_actionCount--;
    }
    return ret;
}

void Model::draw(){
    // 通知データ生成
    EVENT_MtoV tmp = {};

    m_supplyCards.createData(&tmp.supplyCards);
    if(m_phase != PHASE_OTHERPLAYER){
        m_playerCards.createData(&tmp.playerCards);
    }
    else{
        m_playerCards.createDataOtherPlayerCards(&tmp.playerCards);
    }
    m_trashCards.createData(&tmp.trashCards);
    tmp.abcCount.action = m_actionCount;
    tmp.abcCount.buy = m_buyCount;
    tmp.abcCount.coin = m_coinCount;
    tmp.phase = m_phase;

    m_observer->update(&tmp);
}
