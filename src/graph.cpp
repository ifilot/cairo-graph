#include "graph.h"

Graph::Graph(const unsigned int &_ix, const unsigned int &_iy,
             const unsigned int &_tgrid) {
    this->ix = _ix;
    this->iy = _iy;
    this->bx = 50;
    this->by = 20;
    this->bxr = 20;
    this->byu = 20;
    this->tgrid = _tgrid;
    this->plt = new Plotter(this->ix + this->bx + this->bxr,
                            this->iy + this->by + this->byu);
}

void Graph::set_data(const DATACON &_data) {
    this->data = _data;
    this->find_min();
    this->find_max();
    this->dx = (double)(this->ix) / (this->xmax - this->xmin);
    this->dy = (double)(this->iy) / (this->ymax - this->ymin);
}

void Graph::find_min() {
    for(DATACON::iterator it = data.begin(); it != data.end(); ++it) {
        this->xmin = std::min(it->first, this->xmin);
        this->ymin = std::min(it->second, this->ymin);
    }
}

void Graph::find_max() {
    for(DATACON::iterator it = data.begin(); it != data.end(); ++it) {
        this->xmax = std::max(it->first, this->xmax);
        this->ymax = std::max(it->second, this->ymax);
    }
}

void Graph::plot(const std::string &_filename) {
    this->plot_grid();
    this->plot_graph_border();
    this->plot_points();
    this->plot_ticks();
    this->plt->write(_filename.c_str());
}

void Graph::plot_grid() {
    unsigned int _dx = (this->ix) / 10;
    unsigned int _dy = (this->iy) / 10;

    // horizontal lines
    for(unsigned int i=0; i<5; i++) {
        this->plt->draw_line(
            this->bx,               // start x
            this->byu + 2 * i * _dy, // start y
            this->ix + this->bx,    // dx
            this->byu + 2 * i * _dy,     // dy
            Color(224,224,224),
            0.5);           // color
    }

    // vertical lines
    for(unsigned int i=0; i<5; i++) {
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
        1.0);                   // line width
}

void Graph::plot_ticks() {
    unsigned int _dy = (this->iy) / ((this->tgrid -1) * 2.0);
    unsigned int _dx = (this->ix) / ((this->tgrid - 1) * 2.0);
    char buffer[10];
    const float fontsize = 10;
    const char* format = "%0.1e";

    for(unsigned int i=0; i<this->tgrid; i++) {
        float x = this->bx - 50.0;
        float y = this->iy + this->byu - 2.0 * i * _dy + 2.5;
        float value = this->ymin + (double)i * (this->ymax - this->ymin) / 5.0f;
        this->plt->type(x, y, fontsize, float2str2(value, format));
    }

    for(unsigned int i=0; i<this->tgrid; i++) {
        float x = this->bx + 2.0 * i * _dx - 22;
        float y = this->iy + this->byu + 15;
        float value = this->xmin + (double)i * (this->xmax - this->xmin) / 5.0f;
        this->plt->type(x, y, fontsize, float2str2(value, format));
    }
}

void Graph::plot_points() {
    for(DATACON::iterator it = data.begin(); it != data.end(); ++it) {
        this->plt->draw_filled_circle(
            this->bx + (it->first - this->xmin) * this->dx,     // cx
            this->iy + this->byu - (it->second - this->ymin) * this->dy,    // cy
            2,                                                  // radius
            Color(0,0,0));                                      //color
    }
}
