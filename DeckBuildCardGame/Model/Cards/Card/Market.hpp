#ifndef Market_hpp
#define Market_hpp

#include "Card.hpp"


//////////////////////////////
// 祝祭クラス
//////////////////////////////
class Market : virtual public Card{
public:
    Market();
    ~Market();
    ActionDone action(PLAYERCARD_CONCRETECARD_IO *p);
    const string* who();
private:
};

#endif /* Market_hpp */
