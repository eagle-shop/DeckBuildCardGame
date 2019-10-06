#ifndef Smithy_hpp
#define Smithy_hpp

#include "Card.hpp"

//////////////////////////////
// 鍛冶屋クラス
//////////////////////////////
class Smithy : virtual public Card{
public:
    Smithy();
    ~Smithy();
    ActionDone action(PLAYERCARD_CONCRETECARD_IO *p);
    const std::string* who();
private:
};

#endif /* Smithy_hpp */
