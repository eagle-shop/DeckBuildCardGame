#ifndef Model_hpp
#define Model_hpp

#include <vector>
#include "Observer.hpp"
#include "EventCtoM.hpp"
#include "SupplyCards.hpp"
#include "PlayerCards.hpp"
#include "TrashCards.hpp"

using namespace std;

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
    Observer_forView        *_observer;     // Viewへの通知用
    SupplyCards             _supplyCards;   // サプライカードクラスインスタンス
    PlayerCards             _playerCards;   // プレイヤーカードクラスインスタンス
    TrashCards              _trashCards;    // 廃棄カードクラスインスタンス
    Phase                   _phase;         // フェーズ（状態）
    ACTIONNUM               _actionCount;   // 残りアクション数
    BUYNUM                  _buyCount;      // 残り購入数
    COINNUM                 _coinCount;     // コイン数
};

#endif /* Model_hpp */
