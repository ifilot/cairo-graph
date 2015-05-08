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

#include "plotter.h"
#include "lexical_casts.h"

// create special type
typedef std::vector<std::pair<double, double> > DATACON;

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
public:
    Graph(const unsigned int &_ix, const unsigned int &_iy);
    void set_data(const DATACON *_data);
    void plot(const std::string &_filename);

private:
    void find_min();
    void find_max();
    void find_dimensions();
    void plot_points();
    void plot_lines();
    void plot_grid();
    void plot_ticks();
    void plot_graph_border();
};

#endif //_GRAPH_H
