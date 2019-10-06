#ifndef Card_hpp
#define Card_hpp

#include <vector>
#include <string>
#include "General.hpp"
#include "Card_s.hpp"

//////////////////////////////
// カードインタフェースクラス
//////////////////////////////

class Card{
public:
    virtual ~Card(){};
    virtual ActionDone action(PLAYERCARD_CONCRETECARD_IO *p) = 0;
    virtual const std::string* who() = 0;
};

#endif /* Card_hpp */
