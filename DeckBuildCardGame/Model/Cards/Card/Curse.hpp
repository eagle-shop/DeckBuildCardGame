#ifndef Curse_hpp
#define Curse_hpp

#include "Card.hpp"

//////////////////////////////
// 公領クラス
//////////////////////////////
class Curse : virtual public Card{
public:
    Curse();
    ~Curse();
    ActionDone action(PLAYERCARD_CONCRETECARD_IO *p);
    const string* who();
};

#endif /* Curse_hpp */
