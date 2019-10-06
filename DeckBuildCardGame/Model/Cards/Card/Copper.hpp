#ifndef Copper_hpp
#define Copper_hpp

#include "Card.hpp"

//////////////////////////////
// 銅貨クラス
//////////////////////////////
class Copper : virtual public Card{
public:
    Copper();
    ~Copper();
    ActionDone action(PLAYERCARD_CONCRETECARD_IO *p);
    const std::string* who();
};

#endif /* Copper_hpp */
