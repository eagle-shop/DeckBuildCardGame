#ifndef PlayerCards_hpp
#define PlayerCards_hpp

#include <vector>
#include <string>
#include "General.hpp"
#include "EventMtoV.hpp"
#include "Card.hpp"

using namespace std;

typedef struct{
    vector<Card*> handCards;
    vector<DrawState> drawState;
}HAND_DATA;

//////////////////////////////
// プレイヤーカードクラス
//////////////////////////////
class PlayerCards{
public:
    //////////////////////////////
    // パブリックメソッド
    //////////////////////////////
    PlayerCards();
    ~PlayerCards();
    void init();
    void createData(PLAYERCARDS *playerCards);
    void allPrint();
    void buy(const string *name);
    void cleanup();
    ActionDone action(MODEL_PLAYERCARD_IO* modelPlayerCard_ioData);
    ActionDone actionForWaitAction(MODEL_PLAYERCARD_IO* modelPlayerCard_ioData);
    COINNUM treasureMoveToPlayArea(CARDINDEX cardindex);
    const string* removeCard(AREA* area, CARDINDEX cardIndex);
    void addPlayCardOtherPlayer(const string *name);
    void clearPlayCardOtherPlayer();
    void createDataOtherPlayerCards(PLAYERCARDS *playerCards);

private:
    //////////////////////////////
    // プライベート変数
    //////////////////////////////
    vector<Card*>           _deckCards;             // 山札（テーブルに近いほうから0）
    HAND_DATA               _handCards;             // 手札
    vector<Card*>           _discardCards;          // 捨札（テーブルに近いほうから0）
    vector<Card*>           _playCards;             // プレイエリア
    vector<Card*>           _tmpCards;              // 一時置き場
    vector<const string*>   _playCardsOtherPlayer;  // 相手のプレイエリア

    //////////////////////////////
    // プライベートメソッド
    //////////////////////////////
    bool shuffleDiscardToDeck();    // 捨札をシャッフルして山札に移動
    void moveDeckToHand(CARDNUM cardNum);    // 山札から手札に移動
    CARDNUM moveCards(vector<Card*> *dst, vector<Card*> *src, CARDNUM moveNum, Direction direction);    // 指定枚数をまとめてカード移動
    bool moveCards(vector<Card*> *dst, vector<Card*> *src, CARDINDEX index);    // カード1枚移動（連続操作は降順でやること）
};

#endif /* PlayerCards_hpp */
