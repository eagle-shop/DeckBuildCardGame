#ifndef PlayerCards_hpp
#define PlayerCards_hpp

#include <vector>
#include <string>
#include "General.hpp"
#include "EventMtoV.hpp"
#include "Card.hpp"

typedef struct{
    std::vector<Card*> handCards;
    std::vector<DrawState> drawState;
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
    void createData(PLAYERCARDS *playerCards);
    void allPrint();
    void buy(const std::string *name);
    void cleanup();
    ActionDone action(MODEL_PLAYERCARD_IO* modelPlayerCard_ioData);
    ActionDone actionForWaitAction(MODEL_PLAYERCARD_IO* modelPlayerCard_ioData);
    COINNUM treasureMoveToPlayArea(CARDINDEX cardindex);
    const std::string* removeCard(AREA* area, CARDINDEX cardIndex);
    void addPlayCardOtherPlayer(const std::string *name);
    void clearPlayCardOtherPlayer();
    void createDataOtherPlayerCards(PLAYERCARDS *playerCards);

private:
    //////////////////////////////
    // プライベート変数
    //////////////////////////////
    std::vector<Card*>              m_deckCards;            // 山札（テーブルに近いほうから0）
    HAND_DATA                       m_handCards;            // 手札
    std::vector<Card*>              m_discardCards;         // 捨札（テーブルに近いほうから0）
    std::vector<Card*>              m_playCards;            // プレイエリア
    std::vector<Card*>              m_tmpCards;             // 一時置き場
    std::vector<const std::string*> m_playCardsOtherPlayer; // 相手のプレイエリア

    //////////////////////////////
    // プライベートメソッド
    //////////////////////////////
    bool shuffleDiscardToDeck();    // 捨札をシャッフルして山札に移動
    void moveDeckToHand(CARDNUM cardNum);    // 山札から手札に移動
    CARDNUM moveCards(std::vector<Card*> *dst, std::vector<Card*> *src, CARDNUM moveNum, Direction direction);    // 指定枚数をまとめてカード移動
    bool moveCards(std::vector<Card*> *dst, std::vector<Card*> *src, CARDINDEX index);    // カード1枚移動（連続操作は降順でやること）
};

#endif /* PlayerCards_hpp */
