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

/** Declaration used by each of the other modules
 */

#ifndef GLOBAL_DEFS
#define GLOBAL_DEFS

#include <cstdint>
#include <stdio.h>
#include <iterator>
#include <tuple>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <limits>

namespace eldes{
#define DEBUG

  //using UINT = int16_t;
  using INT = uint64_t;
  
  const std::string cAlphabet = "ACGTUNRDQEHILKMFPSWYVB";
  const std::string cElastic_PROTAlphabet = "ACDEFGHIKLMNPQRSTUVWY";
  const std::string cDegenerate_PROTAlphabet = "ABCDEFGHIJKLMNPQRSTUVWXYZ";
  const std::string cElastic_DNAAlphabet = "ACGTU";
  const std::string cDegenerate_DNAAlphabet = "ACGTUNRDHKMSWYVB";

  //  const char Degenerate_Delimiter = '\0';
  const INT cAlphabet_Size = cAlphabet.size();
  const int cString_Delimiter_Start = static_cast<char>(cAlphabet_Size + 1);
  const int cString_Delimiter_End = std::numeric_limits<int>::max();
  const int cMax_Num_Delimiters = cString_Delimiter_End - cString_Delimiter_Start;

  enum class ReturnStatus{
    SUCCESS,
      ERR_ARGS,
      ERR_FILE_OPEN,
      ERR_INVALID_INPUT,
      ERR_INVALID_INDEX,
      ERR_LIMIT_EXCEEDS,
      HELP
      };

  enum class AlphabetType{
    DNA,
      PROT
      };
  
  enum class Region{
    SEED,
      ELASTIC_SYMBOL
      };
  
  using SEED = std::string;
  using SEEDS = std::vector<SEED>;
  using ELASTIC_SYMBOL = std::vector<std::string>;
  using ELASTIC_SYMBOLS = std::vector<ELASTIC_SYMBOL>;
  using PATTERN = std::pair<std::string, std::string>;
  using PATTERNS = std::vector<PATTERN>;

  using POSITION_IN_ELASTIC_STRING = INT;

  using OCCURRENCE = struct Occs{
    POSITION_IN_ELASTIC_STRING start; // starting position of the occurrence
    POSITION_IN_ELASTIC_STRING end; // ending-position of the occurrence
  };
  
  using OCCURRENCES = std::list<OCCURRENCE>;
  using ALL_PATTERNS_OCCURRENCES = std::vector<OCCURRENCES>;

} // end namespace

#endif
