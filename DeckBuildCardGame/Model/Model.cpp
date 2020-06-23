#include <iostream>
#include "Model.hpp"

using namespace DeckBuildCardGameModel;

void testCallback(std::vector<Set> &out_data) {
    for(const auto& e : out_data) {
        std::cout << e.name << std::endl;
    }
}

Model::Model() {
    std::cout << "Model" << std::endl;
}

Model::~Model() {
    std::cout << "~Model" << std::endl;
}

void Model::action() {
    std::cout << "action" << std::endl;
}

std::vector<Set> Model::getSetsData() {
    std::cout << "getData" << std::endl;

    // test
    std::vector<Set> test;
    std::cout << "===== test sync =====" << std::endl;
    m_data.getAllSetData(test);
    for(const auto& e : test) {
        std::cout << e.name << std::endl;
    }

    std::cout << std::endl << "===== test async =====" << std::endl;
    m_data.getAllSetData(testCallback);
    return test;
}
