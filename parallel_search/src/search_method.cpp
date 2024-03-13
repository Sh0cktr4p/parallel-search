#include "search_method.h"


SearchMethod::SearchMethod(std::vector<std::string>& data): data{data} {}

LinearSearchMethod::LinearSearchMethod(std::vector<std::string>& data): SearchMethod{data} {}

std::vector<std::string> LinearSearchMethod::search(std::string &keyword) {
    std::vector<std::string> results;

    for (std::string &element : this->data) {
        if(element.rfind(keyword, 0) == 0) {
            results.push_back(element);
        }
    }

    return results;
}

TreeSearchMethod::TreeSearchMethod(std::vector<std::string>& data): SearchMethod(data), tree() {
    this->tree.addElements(this->data);
}

std::vector<std::string> TreeSearchMethod::search(std::string &keyword) {
    return this->tree.find(keyword);
}
