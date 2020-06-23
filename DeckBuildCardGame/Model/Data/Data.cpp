#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include "Data.hpp"

using namespace DeckBuildCardGameModel;

Data::Data() {
    std::cout << "Data" << std::endl;

    // test
/*    m_sets.push_back({"test1"});
    m_sets.push_back({"test2"});
    m_sets.push_back({"test3"});
    m_sets.push_back({"test4"});
*/}

Data::~Data() {
    std::cout << "~Data" << std::endl;
}

void Data::getAllSetData(std::vector<Set> &out_data) {
    for(const auto& e : m_sets) {
        out_data.push_back(e);
    }
}

void Data::getAllSetData(std::function<void(std::vector<Set> &out_data)> callback) {
    std::vector<Set> out_data;
    for(const auto& e : m_sets) {
        out_data.push_back(e);
    }
    callback(out_data);
}

bool Data::createSet(std::string name) {
    m_sets.push_back({name});
    return true;
}
