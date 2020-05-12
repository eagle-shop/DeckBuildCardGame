#ifndef Model_hpp
#define Model_hpp

#include <vector>
#include "Observer.hpp"
#include "EventCtoM.hpp"
#include "SupplyCards.hpp"
#include "PlayerCards.hpp"
#include "TrashCards.hpp"

constexpr unsigned long ALLCARDS = ULONG_MAX;

//////////////////////////////
// Modelクラス
//////////////////////////////
class Model : virtual public Observer_forModel{
public:
    //////////////////////////////
    // パブリックメソッド
    //////////////////////////////
    Model();
    ~Model();
    void addObserver(Observer_forView *observer);
    Phase update(EVENT_CtoM *event);
    void init();
    static void setTime(TIME time);

private:
    //////////////////////////////
    // プライベートメソッド
    //////////////////////////////
    void actionPhase(EVENT_CtoM *event);
    void actionWaitPhase(EVENT_CtoM *event);
    void buyPhase(EVENT_CtoM *event);
    void cleanupPhase();
    void createActionIoData(EVENT_CtoM *event, MODEL_PLAYERCARD_IO *ioData);
    ActionDone action(EVENT_CtoM *event);
    void draw();

    //////////////////////////////
    // プライベート変数
    //////////////////////////////
    Observer_forView        *m_observer;    // Viewへの通知用
    SupplyCards             m_supplyCards;  // サプライカードクラスインスタンス
    PlayerCards             m_playerCards;  // プレイヤーカードクラスインスタンス
    TrashCards              m_trashCards;   // 廃棄カードクラスインスタンス
    Phase                   m_phase;        // フェーズ（状態）
    ACTIONNUM               m_actionCount;  // 残りアクション数
    BUYNUM                  m_buyCount;     // 残り購入数
    COINNUM                 m_coinCount;    // コイン数
};

#endif /* Model_hpp */
