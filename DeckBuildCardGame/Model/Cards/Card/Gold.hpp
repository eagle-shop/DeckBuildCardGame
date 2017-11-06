#ifndef Gold_hpp
#define Gold_hpp

#include "Card.hpp"

//////////////////////////////
// 金貨クラス
//////////////////////////////
class Gold : virtual public Card{
public:
    Gold();
    ~Gold();
    ActionDone action(PLAYERCARD_CONCRETECARD_IO *p);
    const string* who();
};

#endif /* Gold_hpp */
