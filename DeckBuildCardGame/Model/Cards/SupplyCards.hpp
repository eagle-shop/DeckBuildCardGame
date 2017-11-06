#ifndef SupplyCards_hpp
#define SupplyCards_hpp

#include <vector>
#include <string>
#include "EventMtoV.hpp"
#include "Card.hpp"

using namespace std;

class SupplyCards{
public:
    SupplyCards();
    ~SupplyCards();
    void init();
    void createData(vector<SUPPLYCARDS> *supplyCards);
    COINRELATIVE buy(const string *name, COINNUM coin);
    void buyOtherPlayer(const string *name);

private:
    vector<SUPPLYCARDS> _supplyCards;
};

#endif /* SupplyCards_hpp */
