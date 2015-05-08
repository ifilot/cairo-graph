#include <iostream>
#include <vector>
#include <utility>
#include "graph.h"

int main() {
    DATACON data;

    data.push_back(std::pair<double, double>(-5,-10));
    data.push_back(std::pair<double, double>(1,1.8468));
    data.push_back(std::pair<double, double>(2,4.38486));
    data.push_back(std::pair<double, double>(3,9.0056));
    data.push_back(std::pair<double, double>(4,16.468486));
    data.push_back(std::pair<double, double>(5,25.468));
    data.push_back(std::pair<double, double>(6,36.5456));

    Graph graph(100, 100);
    graph.set_data(data);
    graph.plot("test.png");

    return 0;
}
