#include "parser.h"

Parser::Parser() {
    this->data = new DATACON;
}

void Parser::get_dataset_from_file(std::string filename) {
    std::string input, line;

    std::ifstream infile(filename.c_str());
    while (std::getline(infile, line)) {
        input.append(line).append("\n");
    }
    this->get_dataset_from_string(input);
}

void Parser::get_dataset_from_string(std::string input) {
    const char *pattern = "^\\s*([0-9.eE-]+)\\s+([0-9.eE-]+).*$";
    const char *pcre_error_string;
    const char *pcre_substring_match_string;
    int pcre_error_offset = 0;
    int pcre_exec_ret = 0;
    int pcre_substring_vec[30];

    pcre* regex_compiled = pcre_compile(pattern, 0, &pcre_error_string, &pcre_error_offset, NULL);
    pcre_extra* pcre_extra_p = pcre_study(regex_compiled, 0, &pcre_error_string);

    std::stringstream stream;
    stream << input;
    std::string line;
    while (std::getline(stream, line)) {

        if(line.empty()) {
            break;
        }

        pcre_exec_ret = pcre_exec(regex_compiled, pcre_extra_p, line.c_str(),
                                  line.length(),0, 0, pcre_substring_vec, 30);
        pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret,
                           1, &(pcre_substring_match_string));
        float x = atof(pcre_substring_match_string);
        pcre_get_substring(line.c_str(), pcre_substring_vec, pcre_exec_ret,
                           2, &(pcre_substring_match_string));
        float y = atof(pcre_substring_match_string);

        this->data->push_back(std::pair<double, double>(x,y));
    }
}

const DATACON* Parser::get_dataset() const {
    return this->data;
}
