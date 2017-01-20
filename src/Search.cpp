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

/** Implements class Elastic_string
 */
#include "../include/Search.hpp"

namespace eldes {

  Search::Search(const Elastic_string& e, const PATTERNS& p)
    : _els(e)
    , _patterns(p)
    , _seed_start_positions(e.get_numberof_seeds()+p.size())
    , _elastic_start_indices(e.get_numberof_elastic_symbols()+1)
    , _seeds_searchds(Search::SearchDS{})
    , _elastic_symbols_searchds(Search::SearchDS{}) {
  }

  ReturnStatus Search::find(ALL_PATTERNS_OCCURRENCES& apo) {
    //std::cout << "Search started. " << std::endl;
    int total_pattern_len = 0;
    for (auto p: _patterns) {
      total_pattern_len += p.second.size();
    }
    /* Preprocess */
    ReturnStatus rs = preprocess_seeds(total_pattern_len);
    if ( rs != ReturnStatus::SUCCESS) {
      return rs;
    }

    rs = preprocess_elastic_symbols(total_pattern_len);
    if ( rs != ReturnStatus::SUCCESS) {
      return rs;
    }
    std::cout << "Preprocessing completed \n";
    /* Find each of the _patterns */
    for (INT i=0; i < _patterns.size(); ++i) {
      // Find occurerences of this pattern
      OCCURRENCES occurrence_positions = find_single_pattern(i);
      apo[i] = std::move(occurrence_positions);
    } // end of finding pattern p    
    // std::cout << "Search completed. " << std::endl;
    return ReturnStatus::SUCCESS;
  }


  //////////////////////// private ////////////////////////
  
  ReturnStatus Search::preprocess_seeds(const int total_pattern_size) {
    //std::cout << "Preprocessing of seeds started. " << std::endl;
    const SEEDS& seeds = _els.get_seeds();
    INT num_seeds = seeds.size();
    int num_patterns = _patterns.size();
    
    if (num_seeds + num_patterns >= cMax_Num_Delimiters) {
      std::cerr << "Maximum number of seeds and patterns allowed: " << cMax_Num_Delimiters << ". Number of strings present : " << num_seeds + num_patterns << std::endl;
      return ReturnStatus::ERR_LIMIT_EXCEEDS;
    }

    int total_len = _els.get_length()+1; // +1 for delimiter for the last seed
    total_len += total_pattern_size + num_patterns + 1; //pattern and its delimiter; +1 for appending 0 (required by lib for suffix array)
    std::vector<int> combined(total_len, 0);
    int pos=0;
    int delimiter = cString_Delimiter_Start;
    // fill in seeds and record the starting index of each in the combined string
    for (auto ind=0; ind < num_seeds; ++ind) {
      _seed_start_positions[ind] = pos;
      for (auto c: seeds[ind]) {
	combined[pos++] = static_cast<int>(c);
      }
      combined[pos++] = delimiter++;      
    }
    // fill in patterns and record the starting index of each in the combined string
    for (auto ind=0; ind < num_patterns; ++ind) {
      _seed_start_positions[num_seeds+ind] = pos;
      for (auto c: _patterns[ind].second) {
	combined[pos++] = static_cast<int>(c);
      }
      combined[pos++] = delimiter++;      
    }
    combined[pos] = 0; // append zero as required by library for sa construction
    preprocess_helper(combined, delimiter, _seeds_searchds);

    //std::cout << "Preprocessing of seeds completed. " << std::endl;
    return ReturnStatus::SUCCESS;
  }

  
  ReturnStatus Search::preprocess_elastic_symbols(const int total_pattern_size) {
    //std::cout << "Preprocessing of elastic symbols started. " << std::endl;
    const ELASTIC_SYMBOLS& elastic_symbols = _els.get_elastic_symbols();
    INT num_elastic_symbols = elastic_symbols.size();
    const int num_str = _els.get_num_of_strings_in_elastic();
    const int num_patterns = _patterns.size();
    if (num_str + num_patterns  >= cMax_Num_Delimiters) {
      std::cerr << "Maximum number of strings(alleles) allowed in all the  elastic(variable) regions and number of patterns allowed: " << cMax_Num_Delimiters << ". Number of strings present: " << num_str + num_patterns << std::endl;
      return ReturnStatus::ERR_LIMIT_EXCEEDS;
    }
    
    int total_len = _els.get_elastic_size() + num_str; // each string in each elastic symbol followed by a delimiter
    total_len += total_pattern_size + num_patterns + 1; //pattern and its delimiter; +1 for appending 0 (required by lib for suffix array)
    std::vector<int> combined(total_len, 0);
    int pos=0;
    int delimiter = cString_Delimiter_Start;
   // fill in strings of each elastic-degenerate symbiol and record the starting index of each in the combined string
    for (auto ind=0; ind < num_elastic_symbols; ++ind) {
      const ELASTIC_SYMBOL& elsymb= elastic_symbols[ind];
      if (!elsymb.empty()) {
	// Reserve space for starting indices for each of the string in the symbol;
	_elastic_start_indices[ind].reserve(elsymb.size());
	for (auto const& str: elsymb) {
	  _elastic_start_indices[ind].push_back(pos);
	  for (auto c: str) {
	    combined[pos++] = static_cast<int>(c);
	  }
	  combined[pos++] = delimiter++;	 
	} // all strings in the elastic-symbol end
      }
    } // all elastic-symbols end
    // fill in patterns and record the starting index of each in the combined string
    int pattern_strt_ind = num_elastic_symbols; // patterns starting indices are stored as the last array
    _elastic_start_indices[pattern_strt_ind].reserve(num_patterns);
    for (auto ind=0; ind < num_patterns; ++ind) {
      _elastic_start_indices[pattern_strt_ind].push_back(pos);
      for (auto c: _patterns[ind].second) {
	combined[pos++] = static_cast<int>(c);
      }
      combined[pos++] = delimiter++;      
    }
    
    combined[pos] = 0; // append zero as required by library for sa construction
    preprocess_helper(combined, delimiter, _elastic_symbols_searchds);

    //std::cout << "Preprocessing  of elastic symbols completed. " << std::endl;
    return ReturnStatus::SUCCESS;
  }


  void Search::preprocess_helper(const std::vector<int>& generalised, int max_value, Search::SearchDS& searchds) {
    int min_value = 1;
    std::vector<int> sa(generalised.size());
    std::vector<int> lcp(generalised.size());
    std::vector<int> text(generalised);
    // suffix sort gives sa in sa, isa in the given text array
    suffixsort(text.data(), sa.data(), generalised.size()-1, max_value, min_value);

    compute_lcp(generalised, sa, text, lcp);

    searchds.rmq = std::move(sdsl::rmq_succinct_sct<> (&lcp));

    searchds.isa = std::move(text);
    searchds.lcp = std::move(lcp);
  }

  void Search::compute_lcp(const std::vector<int>& text, const std::vector<int>& sa, const std::vector<int>& isa, std::vector<int>& lcp) {
    int l = 0;
    int lowest = 0;
    int i, j2;
    int len = text.size();
    for(int j = 0; j < len; ++j){
      l = std::max(lowest, l-1);  
      i = isa[j];
      if(i != 0){
	j2 = sa[i-1];
	while((j+l < len) && (j2+l < len) && (text[j+l] == text[j2+l])){
	  l++;
	}
      }
      else{
	l = 0;
      }
      lcp[i] = l;
    }
  }
  // Leading and/or trailing empty seeds/strings in an elastic-symbol of an occurrence are ignored and occurrence is reported to start/end in the appropriate elastic-symbol/seed
  OCCURRENCES Search::find_single_pattern(const int pi) {
    const std::string&  patternstr = _patterns[pi].second;
    OCCURRENCES occs;

    const SEEDS& seeds = _els.get_seeds();
    const ELASTIC_SYMBOLS& elastic_symbols = _els.get_elastic_symbols();
    INT num_seeds = seeds.size();
    INT num_elastic_symbols = elastic_symbols.size();
    AlphabetType at = _els.get_alphabet_type();
    //std::vector<INT>& ff = *_failure_function;
    const INT m = patternstr.size();
    
    for (int ind=0; ind < num_seeds; ++ind) {
      /* Search for Type 1: starting of the occurrence in the seed */
      INT s =0;
      const SEED& sd = seeds[ind];
      const INT seed_len = sd.size();
       while (s < seed_len) {
	TICKED_TAILS tt(m, false);
	tt[0] = true;
	std::list<POSITION_IN_ELASTIC_STRING> end_pos = extend_match(pi, ind, s, std::move(tt));
	for (auto e: end_pos) {
	  POSITION_IN_ELASTIC_STRING start{_seed_start_positions[ind] + s};
	  occs.push_back(OCCURRENCE{start, e});
	}
	++s;	
      } // each position of the seed covered
      
      /* Search for Type 2: starting of the occurrence in the elastic region */
      if (ind < num_seeds-1) { // valid elastic region
      	const ELASTIC_SYMBOL& elsymb = elastic_symbols[ind];
      	bool flag_found = false;
      	TICKED_TAILS tt_el(m, false);

      	for (INT el=0; el < elsymb.size(); ++el) {
      	  const auto& str = elsymb[el];
      	  INT str_size = str.size();
	  for (INT i=0; i < str_size; ++i) {
	    INT patInd = 0;
	    Search::SearchIndex si_el {str, _elastic_start_indices[ind][el], i};
	    Search::SearchIndex si_pat {patternstr, _elastic_start_indices[num_elastic_symbols][pi], 0};
	    INT el_match_length = get_longest_match(at, _elastic_symbols_searchds, si_el, si_pat);
	  
	    patInd += el_match_length;
	    
	    if (patInd == m && !flag_found) { // occurrence found
	      flag_found = true;
	      POSITION_IN_ELASTIC_STRING pos {_seed_start_positions[ind+1]-1};
	      occs.push_back(OCCURRENCE{pos, pos});
	    }
	    else if (i+el_match_length == str_size){ // this string hits the boundary of the next seed
	      tt_el[patInd] = true;
	    }
	  } // each position of the string covered

      	} // strings in _elsymb ends
      	std::list<POSITION_IN_ELASTIC_STRING> end_pos_el = extend_match(pi, ind+1, 0, std::move(tt_el));
      	for (auto e: end_pos_el) {
      	  POSITION_IN_ELASTIC_STRING start{_seed_start_positions[ind+1]-1};
      	  occs.push_back({start, e});
      	}
      } // following elastic region ends      
    } // all seeds and the following(if any) elastic symbols covered
    return occs;
  }

  std::list<POSITION_IN_ELASTIC_STRING> Search::extend_match(const int pi, const INT start_seed_ind, const INT start_seed_offset, TICKED_TAILS&& tt_initial) {
    const std::string&  patternstr = _patterns[pi].second;
    std::list<POSITION_IN_ELASTIC_STRING> occ_end_pos;
    const SEEDS& seeds = _els.get_seeds();
    const ELASTIC_SYMBOLS& elastic_symbols = _els.get_elastic_symbols();
    const INT num_seeds = seeds.size();
    const INT num_els = elastic_symbols.size();
    AlphabetType at = _els.get_alphabet_type();

    INT ind =  start_seed_ind;
    INT seed_offset =  start_seed_offset;
    TICKED_TAILS tt_current{std::move(tt_initial)};
    bool flag_extend = true; // flag representing when to break (when there is nothing ticked)

    while (flag_extend) {
      flag_extend = false;
      const SEED& seed = seeds[ind];
      Search::TICKED_TAILS tt_next(patternstr.size(), false);
      bool flag_found = false; // flag is checked so that occurrences endinhg in the same degenerate symbol is reported just once
      for (int pattern_startind = patternstr.size()-1 ; pattern_startind >= 0; pattern_startind--) { // to sort in increasing order of ending
	if (tt_current[pattern_startind]) {
	  INT patInd =pattern_startind;
	  INT s_match_length = 0;
	  if (!seed.empty()) {
	    Search::SearchIndex si_seed {seeds[ind], _seed_start_positions[ind], seed_offset};
	    Search::SearchIndex si_pat {patternstr, _seed_start_positions[num_seeds+pi], patInd};
	    s_match_length = get_longest_match(at, _seeds_searchds, si_seed, si_pat);
	    //std::cout << "\n Finding lcp at seed(" << ind << seed_offset <<") and (" << _seed_start_positions[num_seeds] <<","<< patInd <<")=" <<s_match_length << std::endl;

	  }
	  patInd += s_match_length;
	  if (patInd == patternstr.size() && !seed.empty()) { // occurrence found	
	    POSITION_IN_ELASTIC_STRING end {_seed_start_positions[ind] + seed_offset+s_match_length-1};
	    occ_end_pos.push_back(end);	  
	  }
	
	  else if ((seed_offset+s_match_length) == seed.size() && ind < seeds.size()-1) { // seed-end hit and seed wasn't the last => elastic symbol hit 
	    const ELASTIC_SYMBOL& elsymb = elastic_symbols[ind];
	    //TICKED_TAILS tt;
	    for (auto el=0; el < elsymb.size(); ++el) {
	      INT el_match_length = 0;
	      INT patInd_el = patInd;
	      if (!(elsymb[el].empty())) {
		Search::SearchIndex si_el {elsymb[el], _elastic_start_indices[ind][el], 0};
		Search::SearchIndex si_pat {patternstr, _elastic_start_indices[num_els][pi], patInd_el};
		el_match_length = get_longest_match(at, _elastic_symbols_searchds, si_el, si_pat);
		//std::cout << "\n Finding lcp at el(" << el << ":0) and (" <<_elastic_start_indices[num_els][0] <<","<< patInd_el <<")=" <<el_match_length << std::endl;


	      }
	      patInd_el += el_match_length;
	    
	      if (patInd_el == patternstr.size() && !flag_found) { // occurrence found
	      //if (patInd_el == patternstr.size()) { // occurrence found
		flag_found = true;
		POSITION_IN_ELASTIC_STRING end {_seed_start_positions[ind+1]-1}; // elastic symbol position is 1 before the start of the next seed
		occ_end_pos.push_back(end);
	      }
	      else if (el_match_length == elsymb[el].size()){ // this string hits the boundary of the next seed
		tt_next[patInd_el] = true;
		flag_extend = true;
	      }
	    } // strings in _elsymb ends	    
	  } // attempt to find strings in the following _elsymb ends	  
	} // attempt to find the seed followed by elastic symbol(if any) at this position ends
      } // All positions checked
      
      //set up to extend to the next seed
      ++ind;
      seed_offset = 0;
      tt_current = std::move(tt_next);      
    } // while non-empty ticked-tail list
    return occ_end_pos;    
  }
  
  INT Search::get_longest_match(const AlphabetType at, const Search::SearchDS& searchDS, const Search::SearchIndex& si1, const Search::SearchIndex& si2){
    INT longest_match = 0;
    INT start_index1 = si1.base_index + si1.offset;
    INT start_index2 = si2.base_index + si2.offset;
     
    // Stop just before starting of the next string in the generalised string
    INT stop_index1 = si1.base_index + si1.str.length();
    INT stop_index2 = si2.base_index + si2.str.length();
    INT lcp = 0;
    
   
    while (start_index1 < stop_index1 && start_index2 < stop_index2) {
      lcp = getLCP(start_index1, start_index2, searchDS);
      longest_match += lcp;
      start_index1 += lcp;
      start_index2 += lcp;
      if (start_index1 < stop_index1 && start_index2 < stop_index2) {
	if (is_degenerate_match(at, si1.str[start_index1-si1. base_index], si2.str[start_index2-si2. base_index])) { // mismatch only due to degnerate symbol (invalid)
	  ++start_index1;
	  ++start_index2;
	  ++longest_match;
	}
	else { // Valid mismatch
	  break;
	}
      }
    }
    return longest_match;
  }

  // Assumption: isa, lcp, rmq are valid (only size is being checked)
  INT Search::getLCP(const INT suff1, const INT suff2, const Search::SearchDS& searchds) {
    assert(!searchds.isa.empty());
    assert(!searchds.lcp.empty());
    assert(searchds.rmq.size() > 0);
    INT l_rmq, r_rmq;
    INT r1 = searchds.isa[suff1];
    INT r2 = searchds.isa[suff2];
    if (r1 < r2) {
      l_rmq=r1;
      r_rmq=r2;
    }
    else {
      l_rmq=r2;
      r_rmq=r1;
    }
    INT minIndex = searchds.rmq(l_rmq+1, r_rmq);
    INT lcp = searchds.lcp[minIndex];
    return lcp;
    //std::cout << "LCP: "<< lcp << std::endl;

  }


}// end namespace
