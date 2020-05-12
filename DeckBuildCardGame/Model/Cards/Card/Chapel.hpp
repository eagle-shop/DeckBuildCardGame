#ifndef Chapel_hpp
#define Chapel_hpp

#include "Card.hpp"

//////////////////////////////
// 礼拝堂クラス
//////////////////////////////
class Chapel : virtual public Card{
public:
    Chapel();
    ~Chapel();
    ActionDone action(PLAYERCARD_CONCRETECARD_IO *p);
    const std::string* who();
private:
    std::vector<CARDINDEX> m_select;      // 選択状態のカード番号を保持
};

#endif /* Chapel_hpp */
