#ifndef Estate_hpp
#define Estate_hpp

#include "Card.hpp"

//////////////////////////////
// 屋敷クラス
//////////////////////////////
class Estate : virtual public Card{
public:
    Estate();
    ~Estate();
    ActionDone action(PLAYERCARD_CONCRETECARD_IO *p);
    const string* who();
};

#endif /* Estate_hpp */
