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
