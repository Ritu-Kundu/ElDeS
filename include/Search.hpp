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

/** Defines the class Search.
 * It provides the Search facility (implements the actual algorithm) and encompasses all the utilities required for searching.
 */

#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <cassert>
#include <memory>
//#include <sdsl/suffix_arrays.hpp>
//#include <sdsl/lcp.hpp>
#include <sdsl/rmq_support.hpp>

#include "globalDefs.hpp"
#include "utilDefs.hpp"
#include "Elastic_string.hpp"
#include "qsufsort.hpp"
// TODO: Add SNP support in pattern; map char to int
namespace eldes{

  class Search{
    struct SearchDS {
      std::vector<int> isa;
      std::vector<int> lcp;
      sdsl::rmq_succinct_sct<> rmq;    
    };
    struct SearchIndex {
      const std::string&  str; // reference to string 
      const INT base_index; // starting-index in the generalised string
      const INT offset; // index within the string (zero-based)
    };

    using TICKED_TAILS = std::vector<bool>; 
   
  public:
    ReturnStatus find(ALL_PATTERNS_OCCURRENCES& apo );
    Search(const Elastic_string& els, const PATTERNS& patterns);
    //////////////////////// private ////////////////////////
  private:
    const Elastic_string& _els;
    const PATTERNS& _patterns;

    std::vector<std::vector<INT>> _elastic_start_indices;
    std::vector<INT> _seed_start_positions;
    Search::SearchDS _seeds_searchds;
    Search::SearchDS _elastic_symbols_searchds;
    std::unique_ptr<std::vector<INT>> _failure_function;

    ReturnStatus preprocess_seeds(const int total_pattern_size);
    ReturnStatus preprocess_elastic_symbols(const int total_pattern_size);
    void preprocess_helper(const std::vector<int>& generalised, int max_value, SearchDS& searchds) ;
    void compute_lcp(const std::vector<int>& text, const std::vector<int>& sa, const std::vector<int>& isa, std::vector<int>& lcp);
    
    OCCURRENCES find_single_pattern(const int pi);
    std::list<POSITION_IN_ELASTIC_STRING> extend_match(const int pi, const INT start_seed_ind, const INT start_seed_offset, TICKED_TAILS&& tt_initial);
    INT get_longest_match(const AlphabetType at, const Search::SearchDS& searchDS, const Search::SearchIndex& si1, const Search::SearchIndex& si2);
    INT getLCP(const INT suff1, const INT suff2, const SearchDS& searchds) ;
 
  };

  
} // end namespace
#endif
