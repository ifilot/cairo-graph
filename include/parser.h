/**************************************************************************
 *   parser.cpp                                                          *
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

#ifndef _PARSER_H
#define _PARSER_H

#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <pcrecpp.h>

typedef std::vector<std::pair<double, double> > DATACON;

class Parser {
private:
    DATACON *data;

public:
    Parser();
    void get_dataset_from_file(std::string filename);
    void get_dataset_from_string(std::string input);
    const DATACON* get_dataset() const;

private:
};

#endif //_PARSER_H
