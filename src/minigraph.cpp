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

        // input file
        TCLAP::ValueArg<std::string> arg_input_filename("i","input","Input file (i.e. CHGCAR)",false,"__NONE__","filename");
        cmd.add(arg_input_filename);

        // output file (name of the .png file)
        TCLAP::ValueArg<std::string> arg_output_filename("o","filename","Filename to print to",true,"test.png","filename");
        cmd.add(arg_output_filename);

        // graph title
        TCLAP::ValueArg<std::string> arg_title("t","title","Title for the graph",false,"","title");
        cmd.add(arg_title);

        // add one or more lines at designated y-value and using a specified color
        TCLAP::MultiArg<std::string> arg_colorstrings("c", "colorline", "Draw colored line", false, "colorstring");
        cmd.add(arg_colorstrings);

        // whether to plot lines (default true)
        TCLAP::SwitchArg arg_has_lines("l","lines","plot lines in the graph", cmd, false);

        // whether to plot points (default false)
        TCLAP::SwitchArg arg_has_points("p","points","plot points in the graph", cmd, false);

        // whether to plot bars (default false)
        TCLAP::SwitchArg arg_has_bars("b","bars","plot bars in the graph", cmd, false);

        cmd.parse(argc, argv);

        //**************************************
        // parsing values
        //**************************************
        std::string input_filename = arg_input_filename.getValue();
        std::string output_filename = arg_output_filename.getValue();
        std::vector<std::string> colorstrings = arg_colorstrings.getValue();

        // plot directives
        std::string title = arg_title.getValue();
        bool has_lines = arg_has_lines.getValue();
        bool has_points = arg_has_points.getValue();
        bool has_bars = arg_has_bars.getValue();

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

        // construct graph
        Graph graph(100, 100);

        // parse instructions to graph
        if(!has_lines && !has_points) { // bare minimum is showing lines
            has_lines = true;
        }
        graph.set_title(title);
        graph.set_colorlines(colorstrings);
        graph.set_property(GRAPH_HAS_LINES, has_lines);
        graph.set_property(GRAPH_HAS_POINTS, has_points);
        graph.set_property(GRAPH_HAS_BARS, has_bars);

        // read data and create graph
        graph.set_data(parser.get_dataset());
        graph.plot(output_filename);

        return 0;
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() <<
                     " for arg " << e.argId() << std::endl;
        return -1;
    }
}
