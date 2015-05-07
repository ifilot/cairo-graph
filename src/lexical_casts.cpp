/**************************************************************************
 *   lexical_cast.cpp                                                     *
 *                                                                        *
 *   MYOCASTOR                                                            *
 *                                                                        *
 *   Authors: Ivo Filot                                                   *
 *            Bart Zijlstra                                               *
 *            Robin Broos                                                 *
 *            Emiel Hensen                                                *
 *                                                                        *
 *   (C) Copyright 2015 Inorganic Materials Chemistry                     *
 *                                                                        *
 *   This is a legal licensing agreement (Agreement) between              *
 *   You (an individual or single legal entity) and                       *
 *   Inorganic Materials Chemistry (IMC) governing the in-house use       *
 *   of the MYOCASTOR software product (Software).                        *
 *   By downloading, installing, or using Software, You agree to be bound *
 *   by the license terms as given on http://www.myocastor.nl/license.    *
 *                                                                        *
 *   Routines for converting non-string types to string                   *
 *                                                                        *
 **************************************************************************/

#include "lexical_casts.h"

std::string int2str(const int &i) {
  std::stringstream ss;
  ss << i;
  return std::string(ss.str());
}

std::string float2str(const float &i) {
  std::stringstream ss;
  ss << i;
  return std::string(ss.str());
}

std::string float2str2(const float &i, const char* str) {
    char buffer [100];
    snprintf(buffer, 100, str, i);
    return std::string(buffer);
}

std::string double2str(const double &i) {
  std::stringstream ss;
  ss << i;
  return std::string(ss.str());
}
