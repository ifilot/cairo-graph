/**************************************************************************
 *   graph.cpp                                                            *
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

#include "graph.h"

/*
 * Construct graph object
 *
 * Specify the height and width of the graph
 * See the manual for the exact specifications
 */
Graph::Graph(const unsigned int &_ix, const unsigned int &_iy) {
    this->ix = _ix;     // width of the graph
    this->iy = _iy;     // height of the graph
    this->bx = 50;      // left margin
    this->by = 50;      // bottom margin
    this->bxr = 20;     // right margin
    this->byu = 20;     // top margin
    this->gridlines = 10;       // number of gridlines

    // create plotter object
    this->plt = new Plotter(this->ix + this->bx + this->bxr,
                            this->iy + this->by + this->byu);

    // write down default values for x- and y-range
    this->xmin = 5e8; // some large number
    this->ymin = 5e8; // some large number
    this->xmax = -5e8; // some small number
    this->ymax = -5e8; // some small number
}

/*
 * Load in the data and establish the domain and range of the data
 */
void Graph::set_data(const DATACON *_data) {
    this->data = _data;
    this->find_min();
    this->find_max();
    this->find_dimensions();
}

/*
 * Find the minimum value of x and y
 */
void Graph::find_min() {
    for(DATACON::const_iterator it = data->begin(); it != data->end(); ++it) {
        this->xmin = std::min(it->first, this->xmin);
        this->ymin = std::min(it->second, this->ymin);
    }
}

/*
 * Find the maximum value of x and y
 */
void Graph::find_max() {
    for(DATACON::const_iterator it = data->begin(); it != data->end(); ++it) {
        this->xmax = std::max(it->first, this->xmax);
        this->ymax = std::max(it->second, this->ymax);
    }
}

/*
 * Establish the dimensions of the graph. To have 'nice and round' numbers
 * at the tick points, establish the 'order of magnitude' of the data
 * range and create the range and domain for the graph accordingly.
 */
void Graph::find_dimensions() {
    // establish order of magnitude of 'tick' interval
    double dx = this->xmax - this->xmin;
    double dy = this->ymax - this->ymin;

    // some handling when the domain and range of the data is
    // very close to zero
    if(fabs(dx) < 1e-5) {
        dx = this->xmax * 1.5;
    }

    if(fabs(dy) < 1e-5) {
        dy = this->ymax * 1.5;
    }

    if(fabs(dx) < 1e-5) {
        dx = 2.0;
        this->xmin = -1.0;
        this->xmax = 1.0;
    }

    if(fabs(dy) < 1e-5) {
        dy = 2.0;
        this->ymin = -1.0;
        this->ymax = 1.0;
    }

    // establish order of magnitude
    float oomx = log10(dx);
    float oomy = log10(dy);

    // establish the interval
    this->int_x = pow(10.0f, floor(oomx));
    this->int_y = pow(10.0f, floor(oomy));

    // find upper and lower (round) value for x
    this->xgmin = floor(this->xmin / this->int_x) * this->int_x;
    this->xgmax = ceil(this->xmax / this->int_x) * this->int_x;

    // find upper and lower (round) value for y
    this->ygmin = floor(this->ymin / this->int_y) * this->int_y;
    this->ygmax = ceil(this->ymax / this->int_y) * this->int_y;

    // recalculate increment
    this->dx = (double)(this->ix) / (this->xgmax - this->xgmin);
    this->dy = (double)(this->iy) / (this->ygmax - this->ygmin);
}

/*
 * Write the graph to the file
 */
void Graph::plot(const std::string &_filename) {
    // background and grid
    this->plot_grid();

    // functions depending on properties
    if(this->properties.test(GRAPH_HAS_LINES)) {
        this->plot_lines();
    }
    if(this->properties.test(GRAPH_HAS_POINTS)) {
        this->plot_points();
    }

    // other stuff
    this->plot_ticks();
    this->plot_graph_border();

    this->plt->write(_filename.c_str());
}

/*
 * Utility function to create a grid. The number of gridlines
 * can be set using this->gridlines
 */
void Graph::plot_grid() {
    unsigned int _dx = (this->ix) / (this->gridlines * 2);
    unsigned int _dy = (this->iy) / (this->gridlines * 2);

    // horizontal lines
    for(unsigned int i=0; i<this->gridlines; i++) {
        this->plt->draw_line(
            this->bx,               // start x
            this->byu + 2 * i * _dy, // start y
            this->ix + this->bx,    // dx
            this->byu + 2 * i * _dy,     // dy
            Color(224,224,224),
            0.5);           // color
    }

    // vertical lines
    for(unsigned int i=0; i<this->gridlines; i++) {
        this->plt->draw_line(
            this->bx + 2 * i * _dx,
            this->byu,
            this->bx + 2 * i * _dx,
            this->iy + this->byu,
            Color(224,224,224),
            0.5);
    }
}

/*
 * Create a border around the graph
 */
void Graph::plot_graph_border() {
    this->plt->draw_empty_rectangle(
        this->bx,               // start x
        this->byu,              // start y
        this->ix,               // dx
        this->iy,               // dy
        Color(0,0,0),           // color
        0.25);                   // line width
}

/*
 * Plot the ticks and ticklines
 */
void Graph::plot_ticks() {
    const float fontsize = 10;
    const char* format = "%0.2f";

    // vertical axis ticks
    for(float yy = this->ygmin; yy <= this->ygmax; yy += this->int_y) {
        std::string text = float2str2(yy, format);
        float x = this->bx - text.length() * 7 - 5;
        float y = this->iy + this->byu - (yy - this->ygmin) * this->dy + 0.3 * fontsize;
        this->plt->type(x, y, fontsize, 0, text);

        this->plt->draw_line(
            this->bx,
            this->iy + this->byu - (yy - this->ygmin) * this->dy,
            this->ix + this->bx,
            this->iy + this->byu - (yy - this->ygmin) * this->dy,
            Color(124,124,124),
            0.5);
        this->plt->draw_line(
            this->bx-5,
            this->iy + this->byu - (yy - this->ygmin) * this->dy,
            this->bx,
            this->iy + this->byu - (yy - this->ygmin) * this->dy,
            Color(0,0,0),
            1.0);
    }

    // horizontal axis ticks
    for(float xx = this->xgmin; xx <= this->xgmax; xx += this->int_x) {
        std::string text = float2str2(xx, format);
        float x = this->bx + (xx - this->xgmin) * this->dx - 3;
        float y = this->iy + this->byu + 20 - 0.25 * text.length() * fontsize;
        this->plt->type(x, y, fontsize, 90, text);

        this->plt->draw_line(
            this->bx + (xx - this->xgmin) * this->dx,
            this->byu,
            this->bx + (xx - this->xgmin) * this->dx,
            this->byu + this->iy,
            Color(124,124,124),
            0.5);

        this->plt->draw_line(
            this->bx + (xx - this->xgmin) * this->dx,
            this->byu + this->iy,
            this->bx + (xx - this->xgmin) * this->dx,
            this->byu + this->iy + 5,
            Color(0,0,0),
            1.0);
    }
}

/*
 * Plot the datapoints as small circles
 */
void Graph::plot_points() {
    for(DATACON::const_iterator it = data->begin(); it != data->end(); ++it) {
        this->plt->draw_filled_circle(
            this->bx + (it->first - this->xgmin) * this->dx,     // cx
            this->iy + this->byu - (it->second - this->ygmin) * this->dy,    // cy
            2,                                                  // radius
            Color(0,0,0));                                      //color
    }
}

/*
 * Plot the graph as a set of lines
 */
void Graph::plot_lines() {
    for(DATACON::const_iterator it = data->begin(); it != data->end() - 1; ++it) {
        float x1 = this->bx + (it->first - this->xgmin) * this->dx;
        float y1 = this->iy + this->byu - (it->second - this->ygmin) * this->dy;

        float x2 = this->bx + ((it+1)->first - this->xgmin) * this->dx;
        float y2 = this->iy + this->byu - ((it+1)->second - this->ygmin) * this->dy;

        this->plt->draw_line(x1, y1, x2, y2, Color(0,0,0), 1.0);
    }
}

/*
 * Set properties
 */
void Graph::set_property(unsigned int property, bool value) {
    this->properties[property] = value;
}
