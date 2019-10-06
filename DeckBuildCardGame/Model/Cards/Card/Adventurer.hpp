#ifndef Adventurer_hpp
#define Adventurer_hpp

#include "Card.hpp"

//////////////////////////////
// 祝祭クラス
//////////////////////////////
class Adventurer : virtual public Card{
public:
    Adventurer();
    ~Adventurer();
    ActionDone action(PLAYERCARD_CONCRETECARD_IO *p);
    const std::string* who();
private:
    std::vector<CARDINDEX> _treasure;      // 財宝カード番号を保持
};

#endif /* Adventurer_hpp */
