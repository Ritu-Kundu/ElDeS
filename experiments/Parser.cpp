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

/** Implements class Parser
 */
#include "../include/Parser.hpp"

namespace eldes {
  // white space between numbers implies empty string between
  // empty seeds:possible
  // empty elastic symbols: not possible
  // empty string within a symbol: possible
  ReturnStatus Parser::parse_elastic(const AlphabetType at, const std::string filename, Elastic_string& els) {
  std::ifstream infile(filename);
  std::cout << "Reading file: " << filename << std::endl;
  if(!infile.is_open()){
    std::cerr << "Cannot open input file \n";
    return ReturnStatus::ERR_FILE_OPEN;
  }

  els.set_alphabet_type(at);
  std::string line;
  getline(infile, line);
  if (!line.empty()) {
    auto it = line.begin();
    ReturnStatus rs = ReturnStatus::SUCCESS;
    /* Collect the first seed */
    std::string seed="";
    rs = collect_string(at, line, seed, it);
    if ( rs != ReturnStatus::SUCCESS) {
      return rs;
    }
    els.add_seed(seed);


    while (it != line.end()) {
      /* Collect the first marker (demarkating variable/elastic region) */
      // start of variable region is not odd
      if (*it != '{') {
	std::cerr << "Invalid marker for beginning of variable/elastic region: " << *it << std::endl;
	return ReturnStatus::ERR_INVALID_INPUT;
      }
      ++it;
      /* Collect the strings of the elastic region */
      ELASTIC_SYMBOL elsymb;
      /* Collect the first string of elastic region */
      std::string allele="";
      rs = collect_string(at, line, allele, it);
      if ( rs != ReturnStatus::SUCCESS) {
	return rs;
      }
      elsymb.push_back(allele);
    
      /* Collect the second marker (demarcating alleles/strings within the variable/elastic region) */
      // start of variable region is not even
      if (*it != ',') {
	std::cerr << "Invalid marker for separating alleles/strings within variable/elastic region: " << *it << std::endl;
	return ReturnStatus::ERR_INVALID_INPUT;
      }

      //collect other alleles
      do {
	std::string allele="";
	++it;
	rs = collect_string(at, line, allele, it);
	if (rs != ReturnStatus::SUCCESS) {
	  return rs;
	}
	elsymb.push_back(allele);
    
      }while (*it == ',');

      if (*it != '}') {
	std::cerr << "Invalid ending marker of the variable/elastic region" << *it << std::endl;
	return ReturnStatus::ERR_INVALID_INPUT;
      }
      ++it;
      els.add_elastic_symbol(elsymb);
      /* Collect the following seed */
      std::string seed="";
      rs = collect_string(at, line, seed, it);
      if ( rs != ReturnStatus::SUCCESS) {
	return rs;
      }
      els.add_seed(seed);
    } // end of the elastic region and the following seed
  } // end of line
  else {
    std::cerr << "No Input: Empty File: " << std::endl;
    return ReturnStatus::ERR_INVALID_INPUT;
  }
  std::cout << "File read successfully: " << filename << std::endl;
  return ReturnStatus::SUCCESS;
  }

  
  ReturnStatus Parser::parse_pattern(const AlphabetType at, const std::string filename, PATTERNS& patterns) {
    std::ifstream infile(filename);
    std::cout << "Reading file: " << filename << std::endl;
    if(!infile.is_open()){
      std::cerr << "Cannot open input file \n";
      return ReturnStatus::ERR_FILE_OPEN;
    }
    std::string line;
    // Get the first sequence
    getline(infile, line);
    if (line.empty()) {
      std::cerr << "No Input: Empty File: " << std::endl;
      return ReturnStatus::ERR_INVALID_INPUT;
    }
    int lineNum = 0;
    do {
      ++ lineNum;
      if (!line.empty()) {
	PATTERN pat;
	pat.first = "";
	std::string pattern_value{""};
	do {
	  ++ lineNum;
	  if (line.empty()) {
	    break; // empty line => sequence ends
	  }
	  for (char c: line) {
	    if (isspace(c) || isdigit(c)) {
	      // Ignore
	    }
	    else if (is_valid_degenerate_char(at, c)) {
	      pattern_value += map_char(c);
	    }
	    else {
	      std::cerr << "Invalid Input: Not a FASTA format: Invalid character: " << c <<  "at line number" << lineNum << std::endl;
	      return ReturnStatus::ERR_INVALID_INPUT;
	    }
	  }
	} while (getline(infile, line)); // pattern ends
	if (pattern_value == "") {
	  std::cerr << "Invalid Input: Empty Sequence at line number" << lineNum << std::endl;
	  return ReturnStatus::ERR_INVALID_INPUT;
	}
	pattern_value.shrink_to_fit();
	pat.second = std::move(pattern_value);
	patterns.push_back(std::move(pat));
      }
    } while (getline(infile, line)); // sequence ends
    
    std::cout << "File read successfully: " << filename << std::endl;
    return ReturnStatus::SUCCESS;

  }
  
  //////////////////////// private ////////////////////////


  ReturnStatus Parser::collect_string (const AlphabetType at, std::string& line, std::string& str, std::string::iterator& it) {
    for (; it != line.end(); ++it ) {
      auto c = *it;
      if (is_valid_elastic_char(at, c)) {
	if (c=='E') { // E is representing empty string acc to AF
	  str = "";
	}
	else {
	  str += map_char(toupper(c));
	}
      }
      else if (c=='{' || c=='}' || c == ',') {
	// marker/boundary started
	break; 
      }
      else if (isspace(*it)) {
	// ignore
      }
      else{
	// invalid character
	std::cerr << "Invalid character: " << *it << std::endl;
	return ReturnStatus::ERR_INVALID_INPUT;
      }
    }
    str.shrink_to_fit();
    return ReturnStatus::SUCCESS;
  }
} // end namespace
