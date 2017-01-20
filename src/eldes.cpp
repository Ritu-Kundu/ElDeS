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

/** Module containing main() method.
 */

#include <fstream>
#include <cstdlib>
#include <ctime>
#include<array>

#include "../include/globalDefs.hpp"
#include "../include/Parser.hpp"
#include "../include/Elastic_string.hpp"
#include "../include/Search.hpp"
#include "../include/utilDefs.hpp"


/** Module containing main() method.
 */

using namespace eldes;

int main(int argc, char **argv){

  /* Decode arguments */
  struct InputFlags flags;
  if(decodeFlags (argc, argv, flags ) != ReturnStatus::SUCCESS){
    usage();
    return 1;
  }
  /* Parse the input */
  Elastic_string els;
  Parser::parse_elastic(flags.alphabet_type, flags.text_input_filename, els);
  PATTERNS patterns;
  // For Experiments; uncomment the following and comment out the next line.
  // Parser::parse_pattern(flags.alphabet_type, flags.pattern_input_filename, patterns);
  Parser:: parse_degenerate(flags.alphabet_type, flags.pattern_input_filename, patterns);
  Search search(els, patterns);
 
  /* Find the occurrences of each of the pattern */
  ALL_PATTERNS_OCCURRENCES apo(patterns.size());
  std::clock_t start = clock();
  search.find(apo);
  std::clock_t end = clock();
  
  /* Report */
  std::string filename = flags.output_filename;
  std::ofstream outfile(filename);
  // std::cout << "Writing result in file: " << filename << std::endl;
  if(!outfile.is_open()){
    std::cerr << "Cannot open  file \n";
    return static_cast<int>(ReturnStatus::ERR_FILE_OPEN);
  }
  outfile << "Number of patterns: " <<patterns.size() << "\n";
  outfile << "Time Elapsed for processing: " << static_cast<double>(end - start) / CLOCKS_PER_SEC<< " secs.\n";

  for (int i=0; i < patterns.size(); ++i) {
    outfile << ">" << patterns[i].first << "\n";
    outfile << "#" << apo[i].size() << "\n";
    for (auto const& oc: apo[i]) {
      //outfile << oc.end << "\n";
      outfile << "(" << oc.start << "," << oc.end << ") ; ";
    }
  outfile << "\n\n";
  }
}
