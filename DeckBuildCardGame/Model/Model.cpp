#include <iostream>
#include <random>
#include "General.hpp"
#include "Model.hpp"

TIME _time = 0;// 暫定

Model::Model():
#if 0  // リリース
    _phase(PHASE_OTHERPLAYER),
#else   // デバッグ
    _phase(PHASE_ACTION),
#endif
    _actionCount(1),
    _buyCount(1),
    _coinCount(0){
    cout << "create Model" << endl;
}

Model::~Model(){
    cout << "delete Model" << endl;
}

void Model::addObserver(Observer_forView *observer){
    _observer = observer;
}

Phase Model::update(EVENT_CtoM *event){
    // P2P接続時1度のみ受信（再接続は未対応）
    if(event->eventKind == EVENT_P2P_CONNECTED){
        // 先行後攻の判定（とりあえず時間で判断）
        if((_time != 0) && (_time > event->time)){
            _phase = PHASE_ACTION;
        }
        else if((_time != 0) && (_time < event->time)){
            _phase = PHASE_OTHERPLAYER;
        }
        else{
            // 異常終了（暫定）
            exit(0);
        }
    }
    // P2P接続先からのデータ受信時
    else if(event->eventKind == EVENT_P2P_RECEIVEDATA){
        // 相手のフェーズが終わった場合
        if((_phase == PHASE_OTHERPLAYER) && (event->receiveData.phase == PHASE_OTHERPLAYER)){
            _phase = PHASE_ACTION;
            _playerCards.clearPlayCardOtherPlayer();
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
                _supplyCards.buyOtherPlayer(tmpCardName);
            }
            // カード廃棄
            else if(*tmpAreaName == TRASH){
                _trashCards.addCardOtherPlayer(tmpCardName);
            }
            // プレイエリアにカード移動
            else if(*tmpAreaName == PLAY){
                _playerCards.addPlayCardOtherPlayer(tmpCardName);
            }
        }
    }
    // 自分のフェーズ
    else if(event->eventKind == EVENT_SELF){
        switch(_phase){
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
                _phase = PHASE_OTHERPLAYER;
#if 1   // デバッグ
                _phase = PHASE_ACTION;
#endif
                break;

            default:
                break;
        }
    }
    // 描画要求
    draw();
    _playerCards.allPrint();
    return _phase;
}

void Model::init(){
    // サプライ生成
    _supplyCards.init();
    // プレイヤーカード生成
    _playerCards.init();
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
            _phase = PHASE_BUY;
        }
        // ターン終了の場合
        else if(STR_SEARCH(event->self.opeElement, PHASESWENDTURN)){
            _phase = PHASE_CLEANUP;
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
                if(_actionCount <= 0){
                    _phase = PHASE_BUY;
                }
            }
            // ユーザ操作待ちの場合
            else if(ret == ACTIONWAIT){
                _phase = PHASE_ACTIONWAIT;
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
        if(_actionCount <= 0){
            _phase = PHASE_BUY;
        }
        else{
            _phase = PHASE_ACTION;
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
            _phase = PHASE_CLEANUP;
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
            _coinCount += _playerCards.treasureMoveToPlayArea(event->self.opeIndex);
        }
        return;
    }
    // サプライ押下
    else if(STR_SEARCH(event->self.area, SUPPLY)){
        // コイン数が0以上ならカード購入成功
        auto coin = _supplyCards.buy(event->self.opeElement, _coinCount);
        if(coin >= 0){
            _coinCount = coin;
            _buyCount--;
            _playerCards.buy(event->self.opeElement);
            // 購入数が無くなったら、クリーンアップフェーズへ
            if(_buyCount <= 0){
                _phase = PHASE_CLEANUP;
                update(event);
            }
        }
    }
}

// クリーンアップフェーズ
void Model::cleanupPhase(){
    _playerCards.cleanup();
    _actionCount = 1;
    _buyCount = 1;
    _coinCount = 0;
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
    if(_phase == PHASE_ACTION){
        ret = _playerCards.action(&tmp);
    }
    // アクションでユーザ操作がある場合はこっち
    else if(_phase == PHASE_ACTIONWAIT){
        ret = _playerCards.actionForWaitAction(&tmp);
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

        for(auto i = 0; i < tmp.resultData.cardIndex.size(); i++){
            // プレイヤーの手札からカードを削除してカード名を取得
            auto name = _playerCards.removeCard(&HAND, tmp.resultData.cardIndex[i]);
            // 廃棄置き場に追加
            _trashCards.addCard(name);
        }
    }
    // acbカウント反映
    if((tmp.resultData.action < 0) && (_actionCount <= abs(tmp.resultData.action))){
        _actionCount = 0;
    }
    else{
        _actionCount += tmp.resultData.action;
    }
    if((tmp.resultData.buy < 0) && (_buyCount <= abs(tmp.resultData.buy))){
        _buyCount = 0;
    }
    else{
        _buyCount += tmp.resultData.buy;
    }
    if((tmp.resultData.coin < 0) && (_coinCount <= abs(tmp.resultData.coin))){
        _coinCount = 0;
    }
    else{
        _coinCount += tmp.resultData.coin;
    }
    if((_actionCount > 0) && (ret == ACTIONDONE)){
        _actionCount--;
    }
    return ret;
}

void Model::draw(){
    // 通知データ生成
    EVENT_MtoV tmp = {};

    _supplyCards.createData(&tmp.supplyCards);
    if(_phase != PHASE_OTHERPLAYER){
        _playerCards.createData(&tmp.playerCards);
    }
    else{
        _playerCards.createDataOtherPlayerCards(&tmp.playerCards);
    }
    _trashCards.createData(&tmp.trashCards);
    tmp.abcCount.action = _actionCount;
    tmp.abcCount.buy = _buyCount;
    tmp.abcCount.coin = _coinCount;
    tmp.phase = _phase;

    _observer->update(&tmp);
}
