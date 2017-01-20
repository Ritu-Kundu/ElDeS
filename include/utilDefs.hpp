/**
    ElDeS: Pattern Matching in Elastic-Degenerate strings
    Copyright (C) 2016 Ritu Kundu, Solon P. Pissis 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef UTIL_DEFS
#define UTIL_DEFS

#include <sys/time.h>
#include <getopt.h>
#include <cctype>
#include <cassert>

#include "globalDefs.hpp"

namespace eldes{
struct InputFlags{
  std::string text_input_filename;
  std::string pattern_input_filename;
  std::string output_filename;
  AlphabetType alphabet_type;

};

void usage (void);
ReturnStatus decodeFlags(int argc, char* argv [], struct InputFlags& flags);

  bool is_valid_elastic_char(const AlphabetType at, const char c);
  bool is_valid_degenerate_char(const AlphabetType at, const char c);
  char map_char (const char c);
  char reverse_map (const char c);
  bool is_degenerate_match (const AlphabetType at, const char d, const char c);
  bool is_degenerate_match_DNA(const char a, const char d);
  bool is_degenerate_match_PROT(const char a, const char d);
  

} // end namespace

#endif
