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

/** Defines the class Subgraph.
 * It represents a subgraph (which inherits from the class Graph), creates an underlying DAG, and also assists in filtering out 'unreal' superbubbles.
 */

#ifndef ELASTICSTRING_HPP
#define ELASTICSTRING_HPP

#include <vector>

#include "globalDefs.hpp"
#include "utilDefs.hpp"

// TODO: Add SNP support in pattern; map char to int
namespace eldes{

  class Elastic_string{
    
  public:
    Elastic_string();
    void add_seed(SEED const& seed);
    void add_elastic_symbol(ELASTIC_SYMBOL const& elsymb);
    void set_alphabet_type(AlphabetType at);
    INT get_length() const;
    INT get_size() const;
    INT get_numberof_seeds() const;
    INT get_numberof_elastic_symbols() const;
    int get_elastic_size() const;
    int get_num_of_strings_in_elastic() const;
    const SEEDS& get_seeds() const;
    const ELASTIC_SYMBOLS& get_elastic_symbols() const;
    ReturnStatus get_seed(const INT index, SEED& seed) const;
    ReturnStatus get_elastic_symbol(const INT index, ELASTIC_SYMBOL& elsymb) const ;
    AlphabetType get_alphabet_type() const;


    //////////////////////// private ////////////////////////
  private:
    int _length;
    int _size;
    int _elastic_size;
    int _num_elstrings;
    SEEDS _seeds;
    ELASTIC_SYMBOLS _elastic_symbols;
    AlphabetType alphabet_type;
 
  };

  
} // end namespace
#endif
