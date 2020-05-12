#ifndef TrashCards_hpp
#define TrashCards_hpp

#include <string>
#include <vector>

class TrashCards{
public:
    TrashCards();
    ~TrashCards();
    void addCard(const std::string *name);
    void addCardOtherPlayer(const std::string *name);
    void createData(std::vector<const std::string*> *name);
private:
    std::vector<const std::string*> m_trashCards;
};

#endif /* TrashCards_hpp */
