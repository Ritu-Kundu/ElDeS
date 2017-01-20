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

/** Defines some helper functions */

#include "../include/utilDefs.hpp"

namespace eldes{

  static struct option long_options[] =
    {
      { "alphabet",                required_argument, NULL, 'a' },
      { "text-input-file",         required_argument, NULL, 't' },
      { "pattern-input-file",      required_argument, NULL, 'p' },
      { "output-file",             required_argument, NULL, 'o' },
      { "help",                    no_argument,       NULL, 'h' },
      { NULL,                      0,                 NULL, 0   }
    };

  /** Decode the input flags
   */
  ReturnStatus decodeFlags(int argc, char* argv [], struct InputFlags& flags){
    int args = 0;
    int opt;
    std::string alph;
    /* initialisation */
    while ((opt = getopt_long(argc, argv, "a:it:ip:o:h", long_options, nullptr)) != - 1) {
      switch (opt) {
      case 'a':
	alph = std::string(optarg);
	if (alph == "DNA") {
	  flags.alphabet_type = AlphabetType::DNA;
	}
	else if (alph == "PROT") {
	  flags.alphabet_type = AlphabetType::PROT;
	}
	else {
	  std::cerr << "Invalid command: wrong alphabet type: " << std::endl;  
	  return (ReturnStatus::ERR_ARGS);

	}
	args ++;
	break;	

      case 't':
	flags.text_input_filename =  std::string(optarg);
	args ++;
	break;
	
      case 'p':
	flags.pattern_input_filename =  std::string(optarg);
	args ++;
	break;

      case 'o':	
	flags.output_filename = std::string(optarg);
	args ++;
	break;
	

      case 'h':
	return (ReturnStatus::HELP);
      }
    }
    if ( args < 4 ){
      std::cerr << "Invalid command: Too few arguments: " << std::endl;  
      return (ReturnStatus::ERR_ARGS);
    }
    else{
      return (ReturnStatus::SUCCESS);
    }
  }

  /* 
   * Usage of the tool 
   */
  void usage(void) {
    std:: cout << " Usage: eldes <options>\n";
    std::cout << " Standard (Mandatory):\n";
    std::cout <<  "  -a, --alphabet \t <str> \t \t `DNA' for nucleotide  sequences or `PROT' for protein  sequences. \n" ;
    std::cout <<  "  -t, --text-input-file \t <str> \t \t Input file  name for text (Elastic-degenerate string).\n";
    std::cout <<  "  -p, --pattern-input-file \t <str> \t \t Input file  name for (degenerate) patterns.\n";
    std::cout <<  "  -o, --output-file \t <str> \t \t Output filename.\n";
  }


 
  bool is_valid_elastic_char (const AlphabetType at, const char c){
    if (!isalpha(c)) {
      return false;
    }

    int pos = 0;
    switch (at) {
    case AlphabetType::DNA:
      pos = cElastic_DNAAlphabet.find(toupper(c));
      break;
    case AlphabetType::PROT:
      pos = cElastic_PROTAlphabet.find(toupper(c));
      break;
    }
    if (pos != std::string::npos) {
      return true;
    }
  }

  bool is_valid_degenerate_char (const AlphabetType at, const char c){
    if (!isalpha(c)) {
      return false;
    }
    int pos = 0;
    switch (at) {
    case AlphabetType::DNA:
      pos = cDegenerate_DNAAlphabet.find(toupper(c));
      break;
    case AlphabetType::PROT:
      pos = cDegenerate_PROTAlphabet.find(toupper(c));
      break;
    }
    if (pos != std::string::npos) {
      return true;
    }
    return false;
  }

  
  // Assumes will always be a valid character
  char map_char (const char c){
    char coded_char = c - 'A' + 1;
    return coded_char;
  }


  char reverse_map (const char c){
    char decoded_char = c + 'A' - 1;
    return decoded_char;
  }

bool is_degenerate_match (const AlphabetType at, const char d, const char c){
    bool result = false;
    switch (at) {
    case AlphabetType::DNA:
      result = is_degenerate_match_DNA(d, c);
      break;
    case AlphabetType::PROT:
      result = is_degenerate_match_PROT(d, c);
      break;
    }
    return result;
  }

  // Assumes always valid character
  // degenerate should be the second
  bool is_degenerate_match_DNA(const char a, const char d) {
    // assert(is_valid_degenerate_char(AlphabetType::DNA, d));
    //assert(is_valid_elastic_char(AlphabetType::DNA,a));
    bool result = false;
    switch (reverse_map(d)) {      
    case 'A':
    case 'T':
    case 'G':
    case 'U':
      result = (d == a);
      break;

    case 'N':
      result = true; //A,C,G, T or U
      break;
    case 'R':
      result = (a == map_char('A') || a ==  map_char('G') ); //A or G
      break;
    case 'D':
      result = (a !=  map_char('C')); //!C =A,G, T or U
      break;
    case 'B':
      result = (a !=  map_char('A')); //!A =C,G, T or U
      break;
    case 'H':
      result = (a !=  map_char('G')); //!G =A,C, T or U
      break;
    case 'K':
      result = (a ==  map_char('G') || a ==  map_char('T') || a ==  map_char('U')); //G or T or U
      break;
    case 'M':
      result = (a ==  map_char('A') || a ==  map_char('C') ); //A or C
      break;
    case 'S':
      result = (a ==  map_char('C') || a ==  map_char('G')); //C or G
      break;
    case 'W':
      result = (a ==  map_char('A') || a ==  map_char('T') || a ==  map_char('U')); //A or T or U
      break;
    case 'Y':
      result = (a ==  map_char('C') || a ==  map_char('T') || a ==  map_char('U')); //C or T or U
      break;
    case 'V':
      result = (a ==  map_char('A') || a ==  map_char('C') || a ==  map_char('G') ); //!C =A,C or G
      break;
    defalt:
      result = false;
    }
    return result;
  }

  // Assumes valid chars
  bool is_degenerate_match_PROT(const char a, const char d) {
    // assert(is_valid_degenerate_char(AlphabetType::PROT, d));
    // assert(is_valid_elastic_char(AlphabetType::PROT, a));
    bool result = false;
    switch (reverse_map(d)) {      
    case 'A':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'Y':
      result = (d == a);
      break;

    case 'X':
      result = true; //any
      break;
    case 'B':
      result = (a ==  map_char('D') || a ==  map_char('N') ); //D or N
      break;
    case 'J':
      result = (a ==  map_char('L') || a ==  map_char('I') ); //L or I
      break;
    case 'Z':
      result = (a ==  map_char('E') || a ==  map_char('Q') ); //E or Q
      break;
    defalt:
      result = false;

    }
    return result;
  }

} // end namespace
