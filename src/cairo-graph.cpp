#include <iostream>
#include <vector>
#include <utility>
#include "graph.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    Parser parser;
    parser.read(argv[1]);

    Graph graph(100, 100);
    graph.set_data(parser.get_dataset());
    graph.plot("test.png");

    return 0;
}
