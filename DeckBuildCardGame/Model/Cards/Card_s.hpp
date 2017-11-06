#ifndef Card_s_hpp
#define Card_s_hpp

#include <vector>
#include <string>
#include "General.hpp"

using namespace std;

// 暫定
// アクション時の通知情報
typedef struct{
    const string        *area;          // どのエリアの操作か
    const string        *opeElement;    // 操作対象のカードorボタン
    CARDINDEX           opeIndex;       // 操作対象のカード番号（なければ0）
    TAPORDRAG           TapOrDrag;      // タップかドラッグか
    const string        *dragMoveTo;    // ドラッグならドラッグ先（プレイエリアor廃棄置き場or・・・）
}NOTIFY_DATA;

// アクション時の結果情報
typedef struct{
    OPE_ACTION          ope;
    vector<CARDINDEX>   cardIndex;
    ACTIONRELATIVE      action;
    BUYRELATIVE         buy;
    COINRELATIVE        coin;
}RESULT_DATA;

// アクション時の送受信データ
typedef struct{
    NOTIFY_DATA         notifyData;
    RESULT_DATA         resultData;
}MODEL_PLAYERCARD_IO;

// アクション時の通知情報
typedef struct{
    NOTIFY_DATA         data;
    Phase               actionPhase;
    
}NOTIFY_DATA_EX;// 名前は適当・・・

// アクション時の結果情報
typedef struct{
    RESULT_DATA         data;
    DRAWCARDNUM         drawCard;
}RESULT_DATA_EX;

// アクション時の送受信データ
typedef struct{
    NOTIFY_DATA_EX      notifyData;
    RESULT_DATA_EX      resultData;
}PLAYERCARD_CONCRETECARD_IO;

#endif /* Card_s_hpp */
