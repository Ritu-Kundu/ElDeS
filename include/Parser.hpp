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

/** @file Parser.hpp 
 * @brief Defines the class Subgraph.
 * It represents a subgraph (which inherits from the class Graph), creates an underlying DAG, and also assists in filtering out 'unreal' superbubbles.
 */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <cctype>

#include "Elastic_string.hpp"
#include "globalDefs.hpp"
#include "utilDefs.hpp"

namespace eldes{
  /** Class Parser
   * A Parser contains static methods for parsing elastic-degenerate and (standard) degenerate string.
   */
  class Parser{

  public:
    /** @brief Parses the text of the input file into an elastic-degenerate string.
     *
     * Input is required to be in PRG (Population Reference Genome) format.
     * - An elastic symbol is marked by a unique odd number. 
     *   The odd number will mark its boundaries (beginning and end). 
     *   The strings within this symbol are delimited by a unique even number.
     *   Example in PRG encoding will look like: ACCT 5 ACC 6 AA 6 ATT 5 CCCTA 7 AC 8 A 7 CC
     * - White space characters between letters and between a letter and a number are allowed.
     *   White-space characters between numbers represent an empty string between them.
     * - File can not be empty.
     * @param at Alphabet type //DNA for genomic sequences, PROT for proteins
     * @param filename name of the input file 
     * @param els reference to the elastic-string in which input from the file will be parsed.
     * @see AlphabetType
     * @see cElastic_PROTAlphabet
     * @see cElastic_DNAAlphabet
     * @return execution status // SUCCESS if input is valid, otherwise corresponding error code after logging the error

     */
    static ReturnStatus parse_elastic(const AlphabetType at, const std::string filename, Elastic_string& els);

    /** @brief Parses the text of the pattern file into a pattern.
     *
     * Input is required to be in FASTA format.
     * - File can not be empty.
     * - A sequence can not be empty.
     * @param at Alphabet type //DNA for genomic sequences, PROT for proteins
     * @param filename name of the input file 
     * @param patterns reference to the vector in which parsed patterns from the input will be storedd.
     * @see AlphabetType
     * @see cDegenerate_PROTAlphabet
     * @see cDegenerate_DNAAlphabet
     * @return execution status // SUCCESS if input is valid, otherwise corresponding error code after logging the error

     */

    static ReturnStatus parse_degenerate(const AlphabetType at, const std::string filename, PATTERNS& patterns);


  private:
    /** @brief Collects (and maps its characters to integer codes) the valid string(seed or string within an elastic symbol) from the given line starting at the given location.
     *
     * A valid string contains only valid characters. White-space characters are ignored.
     * In a valid input, reads until a digit is encountered.
     * Returns on encountering an invalid character.
     * @param at Alphabet type //DNA for genomic sequences, PROT for proteins
     * @param line line(a string) from which valid string is to be collected.
     * @param str reference to a string in which the collected string will be returned
     * @param it starting iterator in the line. It will be moved ahead as the string is collected.
     * @see AlphabetType
     * @see cDegenerate_PROTAlphabet
     * @see cDegenerate_DNAAlphabet
     * @return execution status // SUCCESS if input is valid, otherwise corresponding error code after logging the error

     */
    static ReturnStatus collect_string(const AlphabetType at, std::string& line, std::string& str, std::string::iterator& it);
    
    /** @brief Collects the number(seed or string within an elastic symbol) starting at the given location in the given line.
     *
     * A valid number contains only digits. Collected until a White-space character or an alphabet is encountered.
     * @param line line(a string) from which the number is to be collected.
     * @param num reference to an integer in which the collected number will be returned
     * @param it starting iterator in the line. It will be moved ahead as the number is collected.
     * @return execution status // SUCCESS if input is valid, otherwise corresponding error code after logging the error

     */
    static ReturnStatus collect_number(const std::string& line, INT& num, std::string::iterator& it);

 
  };

} // end namespace
#endif
