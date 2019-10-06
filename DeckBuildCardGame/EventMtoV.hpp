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

typedef struct{
    const std::string       *cardName;      // カード名
    CARDNUM                 num;            // 残り枚数
    DrawState               drawState;      // 表示状態
}SUPPLYCARDS;

typedef struct{
    std::vector<const std::string*> handCardsName;  // 手札のカード名
    std::vector<DrawState>          drawState;      // 表示状態
}HANDCARDS;

typedef struct{
    HANDCARDS                       handCards;      // 手札のカード
    std::vector<const std::string*> playCardsName;  // プレイエリアのカード名
//    const string            *discardName;   // 捨札のカード名（一番上だけ）
    CARDNUM                 deckCardNum;    // 山札の枚数
}PLAYERCARDS;

typedef struct{
    ACTIONNUM               action;
    BUYNUM                  buy;
    COINNUM                 coin;
}ABCCOUNT;

typedef struct{
    ABCCOUNT                        abcCount;
    Phase                           phase;
//    const string            *popup;
    std::vector<SUPPLYCARDS>        supplyCards;
    PLAYERCARDS                     playerCards;
    std::vector<const std::string*> trashCards;
}EVENT_MtoV;

#endif /* EventMtoV_hpp */
