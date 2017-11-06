#ifndef Province_hpp
#define Province_hpp

#include "Card.hpp"

//////////////////////////////
// 公領クラス
//////////////////////////////
class Province : virtual public Card{
public:
    Province();
    ~Province();
    ActionDone action(PLAYERCARD_CONCRETECARD_IO *p);
    const string* who();
};

#endif /* Province_hpp */
