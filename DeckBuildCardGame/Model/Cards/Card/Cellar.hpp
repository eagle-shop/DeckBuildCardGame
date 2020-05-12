#ifndef Cellar_hpp
#define Cellar_hpp

#include "Card.hpp"

//////////////////////////////
// 地下貯蔵庫クラス
//////////////////////////////
class Cellar : virtual public Card{
public:
    Cellar();
    ~Cellar();
    ActionDone action(PLAYERCARD_CONCRETECARD_IO *p);
    const std::string* who();
private:
    std::vector<CARDINDEX> m_select;      // 選択状態のカード番号を保持
};

#endif /* Cellar_hpp */
