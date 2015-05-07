#include <vector>
#include <utility>
#include <string>
#include <stdio.h>

#include "plotter.h"
#include "lexical_casts.h"

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
    unsigned int tgrid;
    Plotter *plt;
    DATACON data;
public:
    Graph(const unsigned int &_ix, const unsigned int &_iy,
          const unsigned int &_tgrid);
    void set_data(const DATACON &_data);
    void plot(const std::string &_filename);

private:
    void find_min();
    void find_max();
    void plot_points();
    void plot_grid();
    void plot_ticks();
    void plot_graph_border();
};
