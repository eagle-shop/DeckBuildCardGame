#ifndef Woodcutter_hpp
#define Woodcutter_hpp

#include "Card.hpp"

//////////////////////////////
// 木こりクラス
//////////////////////////////
class Woodcutter : virtual public Card{
public:
    Woodcutter();
    ~Woodcutter();
    ActionDone action(PLAYERCARD_CONCRETECARD_IO *p);
    const std::string* who();
private:
};

#endif /* Woodcutter_hpp */
