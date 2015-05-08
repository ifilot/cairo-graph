#include "graph.h"

Graph::Graph(const unsigned int &_ix, const unsigned int &_iy) {
    this->ix = _ix;
    this->iy = _iy;
    this->bx = 50;
    this->by = 50;
    this->bxr = 20;
    this->byu = 20;
    this->gridlines = 10;
    this->plt = new Plotter(this->ix + this->bx + this->bxr,
                            this->iy + this->by + this->byu);
}

void Graph::set_data(const DATACON *_data) {
    this->data = _data;
    this->find_min();
    this->find_max();
    this->find_dimensions();
}

void Graph::find_min() {
    for(DATACON::const_iterator it = data->begin(); it != data->end(); ++it) {
        this->xmin = std::min(it->first, this->xmin);
        this->ymin = std::min(it->second, this->ymin);
    }
}

void Graph::find_max() {
    for(DATACON::const_iterator it = data->begin(); it != data->end(); ++it) {
        this->xmax = std::max(it->first, this->xmax);
        this->ymax = std::max(it->second, this->ymax);
    }
}

void Graph::find_dimensions() {
    // establish order of magnitude of 'tick' interval
    double dx = this->xmax - this->xmin;
    double dy = this->ymax - this->ymin;

    float oomx = log10(dx);
    float oomy = log10(dy);

    this->int_x = pow(10.0f, floor(oomx));
    this->int_y = pow(10.0f, floor(oomy));

    this->xgmin = floor(this->xmin / this->int_x) * this->int_x;
    this->xgmax = ceil(this->xmax / this->int_x) * this->int_x;

    this->ygmin = floor(this->ymin / this->int_y) * this->int_y;
    this->ygmax = ceil(this->ymax / this->int_y) * this->int_y;

    this->dx = (double)(this->ix) / (this->xgmax - this->xgmin);
    this->dy = (double)(this->iy) / (this->ygmax - this->ygmin);
}

void Graph::plot(const std::string &_filename) {
    this->plot_grid();
    this->plot_graph_border();
    this->plot_lines();
    this->plot_points();
    this->plot_ticks();
    this->plt->write(_filename.c_str());
}

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

void Graph::plot_graph_border() {
    this->plt->draw_empty_rectangle(
        this->bx,               // start x
        this->byu,              // start y
        this->ix,               // dx
        this->iy,               // dy
        Color(0,0,0),           // color
        0.25);                   // line width
}

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

void Graph::plot_points() {
    for(DATACON::const_iterator it = data->begin(); it != data->end(); ++it) {
        this->plt->draw_filled_circle(
            this->bx + (it->first - this->xgmin) * this->dx,     // cx
            this->iy + this->byu - (it->second - this->ygmin) * this->dy,    // cy
            2,                                                  // radius
            Color(0,0,0));                                      //color
    }
}

void Graph::plot_lines() {
    for(DATACON::const_iterator it = data->begin(); it != data->end() - 1; ++it) {
        float x1 = this->bx + (it->first - this->xgmin) * this->dx;
        float y1 = this->iy + this->byu - (it->second - this->ygmin) * this->dy;

        float x2 = this->bx + ((it+1)->first - this->xgmin) * this->dx;
        float y2 = this->iy + this->byu - ((it+1)->second - this->ygmin) * this->dy;

        this->plt->draw_line(x1, y1, x2, y2, Color(0,0,0), 1.0);
    }
}
