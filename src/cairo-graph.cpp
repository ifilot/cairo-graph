#include <iostream>
#include <vector>
#include <utility>
#include "graph.h"

int main() {
    DATACON data;

    data.push_back(std::pair<double, double>(0,0));
    data.push_back(std::pair<double, double>(1,1));
    data.push_back(std::pair<double, double>(2,4));
    data.push_back(std::pair<double, double>(3,9));
    data.push_back(std::pair<double, double>(4,16));
    data.push_back(std::pair<double, double>(5,25));
    data.push_back(std::pair<double, double>(6,36));

    Graph graph(100, 100, 3);
    graph.set_data(data);
    graph.plot("test.png");

    return 0;
}
