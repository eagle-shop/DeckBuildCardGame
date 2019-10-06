#ifndef SupplyCards_hpp
#define SupplyCards_hpp

#include <vector>
#include <string>
#include "EventMtoV.hpp"
#include "Card.hpp"

class SupplyCards{
public:
    SupplyCards();
    ~SupplyCards();
    void init();
    void createData(std::vector<SUPPLYCARDS> *supplyCards);
    COINRELATIVE buy(const std::string *name, COINNUM coin);
    void buyOtherPlayer(const std::string *name);

private:
    std::vector<SUPPLYCARDS> _supplyCards;
};

#endif /* SupplyCards_hpp */
