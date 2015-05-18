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
    this->gridlines = 4;       // number of (sub-)gridlines
    this->fontsize = 10;    // default font size

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

    // also add minimum of colorlines
    for(unsigned int i=0; i<this->colorline_values.size(); i++) {
        this->ymin = std::min(this->colorline_values[i], this->ymin);
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

    // also add maximum of colorlines
    for(unsigned int i=0; i<this->colorline_values.size(); i++) {
        this->ymax = std::max(this->colorline_values[i], this->ymax);
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

    // plot dataset as bars (if set)
    if(this->properties.test(GRAPH_HAS_BARS)) {
        this->plot_bars();
    }

    // plot colorlines (if set)
    this->plot_colorlines();

    // plot regular lines (if set)
    if(this->properties.test(GRAPH_HAS_LINES)) {
        this->plot_lines();
    }

    // plot points (if set)
    if(this->properties.test(GRAPH_HAS_POINTS)) {
        this->plot_points();
    }

    // other stuff
    this->plot_title();
    this->plot_ticks();
    this->plot_graph_border();

    this->plt->write(_filename.c_str());
}

/*
 * Utility function to create a grid. The number of gridlines
 * can be set using this->gridlines
 */
void Graph::plot_grid() {
    float _dx = (this->int_x) / (this->gridlines + 1);
    float _dy = (this->int_y) / (this->gridlines + 1);

    // horizontal lines
    for(float yy = this->ygmin; yy <= this->ygmax; yy += _dy) {
        this->plot_line_internal_coordinates(this->xgmin, yy, this->xgmax, yy,
                                             Color(244,244,244), 1.0);
    }

    // vertical lines
    for(float xx = this->xgmin; xx <= this->xgmax; xx += _dx) {
        this->plot_line_internal_coordinates(xx, this->ygmin, xx, this->ygmax,
                                             Color(244,244,244), 1.0);
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
 * Plot colored lines according to colorlines settings
 */
void Graph::plot_colorlines() {
    for(unsigned int i=0; i<this->colorline_values.size(); i++) {
        float y = this->iy + this->byu - (this->colorline_values[i] - this->ygmin) * this->dy;
        this->plt->draw_line(
            this->bx,
            y,
            this->ix + this->bx,
            y,
            this->colorline_colors[i],
            0.5);
    }
}

/*
 * Plot the ticks and ticklines
 */
void Graph::plot_ticks() {
    const char* format = "%0.2f";

    // vertical axis ticks
    for(float yy = this->ygmin; yy <= this->ygmax; yy += this->int_y) {
        std::string text = float2str2(yy, format);
        float x = this->bx - text.length() * 7 - 5;
        float y = this->iy + this->byu - (yy - this->ygmin) * this->dy + 0.3 * this->fontsize;
        this->plt->type(x, y, this->fontsize, 0, text);

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
        float y = this->iy + this->byu + 20 - 0.25 * text.length() * this->fontsize;
        this->plt->type(x, y, this->fontsize, 90, text);

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
 * Plot a title above the graph
 */
void Graph::plot_title() {
    // do not do anything if there is no title
    if(this->title.length() == 0) {
        return;
    }
    this->plt->type(this->bx,             // x
                    this->fontsize * 1.5, // y
                    this->fontsize * 1.0, // size of font
                    0, //rotation
                    this->title);
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
        this->plot_line_internal_coordinates(it->first, it->second,
                                             (it+1)->first, (it+1)->second,
                                              Color(0,0,0), 1.0);
    }
}

/*
 * Plot the datapoints as small bars
 */
void Graph::plot_bars() {
    const float relative_bar_width = 0.95;
    const float bar_width = ((this->xmax - this->xmin) * this->dx) / (float)(this->data->size() - 1) * relative_bar_width;

    float xstart, bdx, diff;
    for(DATACON::const_iterator it = data->begin(); it != data->end(); ++it) {

        xstart = this->bx + (it->first - this->xgmin) * this->dx - bar_width / 2.0;
        bdx = bar_width;

        // clip the bar width if part of the bar ends up outside the drawing area
        // ... when it is too far to the left
        if(xstart < this->bx) {
            diff = xstart - this->bx;
            xstart = this->bx;
            bdx += diff;
        }

        // ... and when it is too far to the right
        if(xstart+bdx > this->bx + this->ix) {
            diff = this->bx + this->ix - xstart - bdx;
            bdx += diff;
        }

        this->plt->draw_filled_rectangle(
            xstart,
            this->iy + this->byu - (it->second - this->ygmin) * this->dy,
            bdx,
            (it->second - this->ygmin) * this->dy,
            Color(100,100,100));
    }
}

float Graph::internal_to_image_x(float _x) {
    return this->bx + (_x - this->xgmin) * this->dx;
}

float Graph::internal_to_image_y(float _y) {
    return this->iy + this->byu - (_y - this->ygmin) * this->dy;
}

void Graph::plot_line_internal_coordinates(float _x1, float _y1, float _x2, float _y2,
                                           Color col, float lt) {
    float x1 = this->internal_to_image_x(_x1);
    float y1 = this->internal_to_image_y(_y1);
    float x2 = this->internal_to_image_x(_x2);
    float y2 = this->internal_to_image_y(_y2);

    this->plt->draw_line(x1, y1, x2, y2, col, lt);
}

/*
 * Set properties
 */
void Graph::set_property(unsigned int property, bool value) {
    this->properties[property] = value;
}

void Graph::set_title(const std::string &_title) {
    this->title = _title;
}

void Graph::set_colorlines(const std::vector<std::string> &_colorlines) {
    this->colorlines = _colorlines;
    std::string delimiter = "#";
    for(std::vector<std::string>::const_iterator it = this->colorlines.begin();
        it != this->colorlines.end(); ++it) {

        std::string value = it->substr(0, it->find(delimiter));
        std::string rgb = it->substr(it->find(delimiter) + 1);

        this->colorline_values.push_back( str2float(value) );
        Color clr(hex2int(rgb.substr(0,2)),
                  hex2int(rgb.substr(2,2)),
                  hex2int(rgb.substr(4,2)) );
        this->colorline_colors.push_back(clr);
    }
}
