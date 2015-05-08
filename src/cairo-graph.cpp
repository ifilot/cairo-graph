#include <iostream>
#include <vector>
#include <utility>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "parser.h"

void read_from_pipe (int file) {
    FILE *stream;
    int c;
    stream = fdopen(file, "r");
    while ((c = fgetc(stream)) != EOF) {
        putchar (c);
    }
    fclose(stream);
}

int main(int argc, char *argv[]) {
    std::string input;
    Parser parser;

    if(argc == 1) { // read from cin
        std::string line;
        while(std::getline(std::cin, line)) {
            input.append(line).append("\n");
        }
        parser.get_dataset_from_string(input);
    } else { // read from file
        parser.get_dataset_from_file(argv[1]);
    }

    Graph graph(100, 100);
    graph.set_data(parser.get_dataset());
    graph.plot("test.png");

    return 0;
}
