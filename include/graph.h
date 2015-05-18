/**************************************************************************
 *   graph.h                                                              *
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

#ifndef _GRAPH_H
#define _GRAPH_H

#include <vector>
#include <utility>
#include <string>
#include <stdio.h>
#include <bitset>

#include "plotter.h"
#include "lexical_casts.h"

// create special type
typedef std::vector<std::pair<double, double> > DATACON;

// define properties
#define GRAPH_HAS_LINES   0 << 0 // 0
#define GRAPH_HAS_POINTS  1 << 0 // 1
#define GRAPH_HAS_BARS    1 << 1 // 2

class Graph {
private:
    double xmin;
    double ymin;
    double xmax;
    double ymax;
    double dx;
    double dy;

    // image / graph properties
    unsigned int ix;
    unsigned int iy;
    unsigned int bx;
    unsigned int by;
    unsigned int bxr;
    unsigned int byu;

    double xgmin;
    double ygmin;
    double xgmax;
    double ygmax;
    double int_x;
    double int_y;
    unsigned int gridlines;
    Plotter *plt;
    const DATACON *data;
    std::bitset<5> properties;
    std::string title;
    float fontsize;
    std::vector<std::string> colorlines;
    std::vector<float> colorline_values;
    std::vector<Color> colorline_colors;

public:
    Graph(const unsigned int &_ix, const unsigned int &_iy);
    void set_data(const DATACON *_data);
    void plot(const std::string &_filename);
    void set_property(unsigned int property, bool value);
    void set_title(const std::string &_title);
    void set_colorlines(const std::vector<std::string> &_colorlines);

private:
    void find_min();
    void find_max();
    void find_dimensions();

    // different plot styles
    void plot_points();
    void plot_lines();
    void plot_bars();

    void plot_title();
    void plot_grid();
    void plot_ticks();
    void plot_graph_border();
    void plot_colorlines();

    float internal_to_image_x(float _x);
    float internal_to_image_y(float _y);
    void plot_line_internal_coordinates(float _x1, float _y1, float _x2, float _y2,
                                        Color col, float lt);
};

#endif //_GRAPH_H
