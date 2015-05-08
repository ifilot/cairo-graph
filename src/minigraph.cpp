/**************************************************************************
 *   minigraph.cpp                                                        *
 *                                                                        *
 *   AUTHOR:  Ivo Filot <ivo@ivofilot.nl>                                 *
 *   PROJECT: MINIGRAPH                                                   *
 *                                                                        *
 *   This program is free software; you can redistribute it and/or modify *
 *   it under the terms of the GNU General Public License as published by *
 *   the Free Software Foundation, version 2                              *
 *                                                                        *
 *   This program is distributed in the hope that it will be useful, but  *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    *
 *   General Public License for more details.                             *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program; if not, write to the Free Software          *
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA            *
 *   02110-1301, USA.                                                     *
 *                                                                        *
 **************************************************************************/

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
    std::string input, output_file;
    Parser parser;

    if(argc == 2) { // read from cin
        std::string line;
        while(std::getline(std::cin, line)) {
            input.append(line).append("\n");
        }
        parser.get_dataset_from_string(input);
        output_file = argv[1];
    } else { // read from file
        parser.get_dataset_from_file(argv[1]);
        output_file = argv[2];
    }

    Graph graph(100, 100);
    graph.set_data(parser.get_dataset());
    graph.plot(output_file);

    return 0;
}
