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
#include <tclap/CmdLine.h>
#include "graph.h"
#include "parser.h"

int main(int argc, char *argv[]) {

    try {
        TCLAP::CmdLine cmd("Creates a tiny graph from a datafile.", ' ', "1.0");

        //**************************************
        // declare values to be parsed
        //**************************************
        TCLAP::ValueArg<std::string> arg_output_filename("o","filename","Filename to print to",true,"test.png","filename");
        cmd.add(arg_output_filename);
        TCLAP::ValueArg<std::string> arg_input_filename("i","input","Input file (i.e. CHGCAR)",false,"__NONE__","filename");
        cmd.add(arg_input_filename);

        cmd.parse(argc, argv);

        //**************************************
        // parsing values
        //**************************************
        std::string input_filename = arg_input_filename.getValue();
        std::string output_filename = arg_output_filename.getValue();

        //**************************************
        // start running the program
        //**************************************

        // construct parser object
        Parser parser;

        if(input_filename == "__NONE__") {
            std::string line, input;
            while(std::getline(std::cin, line)) {
                input.append(line).append("\n");
            }
            parser.get_dataset_from_string(input);
        } else {
            parser.get_dataset_from_file(input_filename);
        }

        Graph graph(100, 100);
        graph.set_data(parser.get_dataset());
        graph.plot(output_filename);

        return 0;
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() <<
                     " for arg " << e.argId() << std::endl;
        return -1;
    }
}
