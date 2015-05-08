#include <iostream>
#include <vector>
#include <utility>
#include "graph.h"
#include "parser.h"

int main() {
    Parser parser;
    parser.read("test/dataset01.dat");

    Graph graph(100, 100);
    graph.set_data(parser.get_dataset());
    graph.plot("test.png");

    return 0;
}
