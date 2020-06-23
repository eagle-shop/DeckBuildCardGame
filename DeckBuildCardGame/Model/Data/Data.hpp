#ifndef Data_hpp
#define Data_hpp

#include <vector>
#include <string>
#include <functional>

namespace DeckBuildCardGameModel {

struct Set {
    std::string name;
};

class Data {
public:
    Data();
    ~Data();
    void getAllSetData(std::vector<Set> &out_data);                                 // sync
    void getAllSetData(std::function<void(std::vector<Set> &out_data)> callback);   // async
    bool createSet(std::string name);

private:
    std::vector<Set> m_sets;
};

}   /* namespace DeckBuildCardGameModel */

#endif /* Data_hpp */
