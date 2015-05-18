/**************************************************************************
 *   plotter.h                                                            *
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

#ifndef _PLOTTER_H
#define _PLOTTER_H

#include <cairo/cairo.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <math.h>

class Color {
private:
  unsigned int r,g,b;
public:
  Color(unsigned int _r, unsigned int _g, unsigned int _b);
  Color(std::string rgb);
  float get_r() const;
  float get_g() const;
  float get_b() const;
};

class ColorScheme {
private:
  std::vector<std::string> scheme;
  std::vector<Color> colors;
  double low, high;
public:
  ColorScheme(const double &_low, const double &_high);
  Color get_color(const double &_value);
private:
  void construct_scheme();
  void convert_scheme();
  Color rgb2color(const std::string &_hex);
  unsigned int hex2int(const std::string &_hex);
};


class Plotter {
private:
  cairo_t *cr;
  cairo_surface_t *surface;
  unsigned int width, height;
  ColorScheme *scheme;
public:
  Plotter(const unsigned int &_width, const unsigned int &_height);
  void set_background(const Color &_color);
  void write(const char* filename);
  void draw_filled_rectangle(float xstart, float ystart, float xstop, float ystop,
                      const Color &_color);
  void draw_empty_rectangle(float xstart, float ystart, float xstop, float ystop,
                      const Color &_color, float line_width);
  void draw_line(float xstart, float ystart, float xstop, float ystop,
                 const Color &_color, float line_width);
  void draw_filled_circle(float cx, float cy, float radius,
                          const Color &_color);
  void draw_empty_circle(float cx, float cy, float radius,
                        const Color &_color, float line_width);
  void type(float x, float y, float fontsize, float rotation, const std::string &_text);
private:

};

#endif //_PLOTTER_H
