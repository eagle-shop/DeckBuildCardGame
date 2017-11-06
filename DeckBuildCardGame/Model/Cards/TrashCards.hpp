#ifndef TrashCards_hpp
#define TrashCards_hpp

#include <string>
#include <vector>

using namespace std;

class TrashCards{
public:
    TrashCards();
    ~TrashCards();
    void addCard(const string *name);
    void addCardOtherPlayer(const string *name);
    void createData(vector<const string*> *name);
private:
    vector<const string*> _trashCards;
};

#endif /* TrashCards_hpp */
