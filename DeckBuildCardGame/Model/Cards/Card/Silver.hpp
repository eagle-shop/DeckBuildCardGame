#ifndef Silver_hpp
#define Silver_hpp

#include "Card.hpp"

//////////////////////////////
// 銀貨クラス
//////////////////////////////
class Silver : virtual public Card{
public:
    Silver();
    ~Silver();
    ActionDone action(PLAYERCARD_CONCRETECARD_IO *p);
    const string* who();
};

#endif /* Silver_hpp */
