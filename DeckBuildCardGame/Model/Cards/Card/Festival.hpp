#ifndef Festival_hpp
#define Festival_hpp

#include "Card.hpp"


//////////////////////////////
// 祝祭クラス
//////////////////////////////
class Festival : virtual public Card{
public:
    Festival();
    ~Festival();
    ActionDone action(PLAYERCARD_CONCRETECARD_IO *p);
    const string* who();
private:
};

#endif /* Festival_hpp */
