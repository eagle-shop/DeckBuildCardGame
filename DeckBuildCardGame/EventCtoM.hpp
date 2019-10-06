#ifndef EventCtoM_hpp
#define EventCtoM_hpp

#include "General.hpp"

typedef struct{
    const std::string   *area;          // どのエリアの操作か
    const std::string   *opeElement;    // 操作対象のカードorボタン
    CARDINDEX           opeIndex;       // 操作対象のカード番号（なければ0）
    TAPORDRAG           TapOrDrag;      // タップかドラッグか
    const std::string   *dragMoveTo;    // ドラッグならドラッグ先（プレイエリアor廃棄置き場or・・・）
}SELF;

typedef struct{
    Phase phase;
    char areaName[STRING_MAX];
    char cardName[STRING_MAX];
}RECEIVEDATA;

typedef struct{
    EVENT_KIND      eventKind;
    TIME            time;
    SELF            self;
    RECEIVEDATA     receiveData;
}EVENT_CtoM;

#endif /* EventCtoM_hpp */
