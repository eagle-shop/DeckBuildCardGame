#ifndef Duchy_hpp
#define Duchy_hpp

#include "Card.hpp"

//////////////////////////////
// 公領クラス
//////////////////////////////
class Duchy : virtual public Card{
public:
    Duchy();
    ~Duchy();
    ActionDone action(PLAYERCARD_CONCRETECARD_IO *p);
    const std::string* who();
};

#endif /* Duchy_hpp */
