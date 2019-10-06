#ifndef Laboratory_hpp
#define Laboratory_hpp

#include "Card.hpp"

//////////////////////////////
// 研究所クラス
//////////////////////////////
class Laboratory : virtual public Card{
public:
    Laboratory();
    ~Laboratory();
    ActionDone action(PLAYERCARD_CONCRETECARD_IO *p);
    const std::string* who();
private:
};

#endif /* Laboratory_hpp */
