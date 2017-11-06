////////////////////////////////////////
// Model -> Viewの通知データ
// メモ：必要なのは描画に必要なデータのみ
// 　　　カード名は全て文字列
////////////////////////////////////////
#ifndef EventMtoV_hpp
#define EventMtoV_hpp

#include <vector>
#include <string>
#include "General.hpp"

using namespace std;

typedef struct{
    const string            *cardName;      // カード名
    CARDNUM                 num;            // 残り枚数
    DrawState               drawState;      // 表示状態
}SUPPLYCARDS;

typedef struct{
    vector<const string*>   handCardsName;  // 手札のカード名
    vector<DrawState>       drawState;      // 表示状態
}HANDCARDS;

typedef struct{
    HANDCARDS               handCards;      // 手札のカード
    vector<const string*>   playCardsName;  // プレイエリアのカード名
//    const string            *discardName;   // 捨札のカード名（一番上だけ）
    CARDNUM                 deckCardNum;    // 山札の枚数
}PLAYERCARDS;

typedef struct{
    ACTIONNUM               action;
    BUYNUM                  buy;
    COINNUM                 coin;
}ABCCOUNT;

typedef struct{
    ABCCOUNT                abcCount;
    Phase                   phase;
//    const string            *popup;
    vector<SUPPLYCARDS>     supplyCards;
    PLAYERCARDS             playerCards;
    vector<const string*>   trashCards;
}EVENT_MtoV;

#endif /* EventMtoV_hpp */
