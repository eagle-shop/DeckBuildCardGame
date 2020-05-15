/**
 * @file    Model.hpp
 * @brief   Core logic
**/
#ifndef Model_hpp
#define Model_hpp

#include <vector>
#include "Observer.hpp"
#include "EventCtoM.hpp"
#include "SupplyCards.hpp"
#include "PlayerCards.hpp"
#include "TrashCards.hpp"

constexpr unsigned long ALLCARDS = ULONG_MAX;

/**
 * @class   Model
**/
class Model : virtual public Observer_forModel{
public:
    /**
     * @brief       Constructor
     * @note        None
    **/
    Model();

    /**
     * @brief       Destructor
     * @note        None
    **/
    ~Model();

    /**
     * @brief       Observer registration method for View
     * @param[in]   observer    : Pointer to View (abstract) class
     * @note        Just copy the pointer
    **/
    void addObserver(Observer_forView *observer);

    /**
     * @brief       User operation notification from controller
     * @param[in]   event       : Notification data
     * @note        Reception of all operations.<br>
     *              Whenever possible, I want to control only the PHASE.
    **/
    Phase update(EVENT_CtoM *event);

    /**
     * @brief       Initialization
     * @note        Calling after the addObserver
    **/
    void init();

    /**
     * @brief       TBD
     * @param[in]   time        : NSDate.timeIntervalSinceReferenceDate when the P2P connection is complete
     * @note        Tentative process until the method for determining the first attack is determined
    **/
    static void setTime(TIME time);

private:
    void actionPhase(EVENT_CtoM *event);
    void actionWaitPhase(EVENT_CtoM *event);
    void buyPhase(EVENT_CtoM *event);
    void cleanupPhase();
    void createActionIoData(EVENT_CtoM *event, MODEL_PLAYERCARD_IO *ioData);
    ActionDone action(EVENT_CtoM *event);
    void draw();

    Observer_forView        *m_observer;    //!< Viewへの通知用
    SupplyCards             m_supplyCards;  //!< サプライカードクラスインスタンス
    PlayerCards             m_playerCards;  //!< プレイヤーカードクラスインスタンス
    TrashCards              m_trashCards;   //!< 廃棄カードクラスインスタンス
    Phase                   m_phase;        //!< フェーズ（状態）
    ACTIONNUM               m_actionCount;  //!< 残りアクション数
    BUYNUM                  m_buyCount;     //!< 残り購入数
    COINNUM                 m_coinCount;    //!< コイン数
};

#endif /* Model_hpp */
