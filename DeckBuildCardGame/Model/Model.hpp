#ifndef Model_hpp
#define Model_hpp

#include "Data.hpp"

namespace DeckBuildCardGameModel {

class Model {
public:
    Model();
    ~Model();
    void action();
    std::vector<Set> getSetsData();
    bool createSet(std::string name){return m_data.createSet(name);};

private:
    Data m_data;
};

}   /* namespace DeckBuildCardGameModel */

#endif /* Model_hpp */
