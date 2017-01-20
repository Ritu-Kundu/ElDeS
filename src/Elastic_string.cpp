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
#include "../include/Elastic_string.hpp"

namespace eldes {
  Elastic_string::Elastic_string()
    : _seeds()
    , _elastic_symbols()
    , _length(0)
    , _size(0)
    , _num_elstrings(0)
    , _elastic_size(0){
  }
  
  void Elastic_string::add_seed(SEED const& seed) {
    _length += seed.size();
    _size += seed.size();   
    _seeds.push_back(std::move(seed));
  }

  void Elastic_string::set_alphabet_type(AlphabetType at) {
    alphabet_type = at;
  }
    
  void Elastic_string::add_elastic_symbol(ELASTIC_SYMBOL const& elsymb) {
    _length += 1; // 1 symbol occupies 1 position
    int sz = 0;
    for (auto e:elsymb) {
      sz += e.size();
    }
    _size += sz;
    _elastic_size += sz;
    _num_elstrings += elsymb.size();
    _elastic_symbols.push_back(std::move(elsymb));    
  }

  INT Elastic_string::get_length() const {
    return _length;
  }

  INT Elastic_string::get_size() const {
    return _size;
  }

  int Elastic_string::get_elastic_size() const {
    return _elastic_size;
  }

  int Elastic_string::get_num_of_strings_in_elastic() const {
    return _num_elstrings;
  }
  
  INT Elastic_string::get_numberof_seeds() const {
    return _seeds.size();
  }

  INT Elastic_string::get_numberof_elastic_symbols() const {
    return _elastic_symbols.size();
  }
  
  const SEEDS& Elastic_string::get_seeds() const {
    return _seeds;
  }

  const ELASTIC_SYMBOLS& Elastic_string::get_elastic_symbols() const {
    return _elastic_symbols;
  }

  ReturnStatus Elastic_string::get_seed(const INT index, SEED& seed) const {
    if (index < 0 || index >= _seeds.size() ) {
      std::cerr << "Invalid index: " << index << std::endl;
      return ReturnStatus::ERR_INVALID_INDEX;
    }
    seed = _seeds[index];
    return ReturnStatus::SUCCESS;
  }

    ReturnStatus Elastic_string::get_elastic_symbol(const INT index,ELASTIC_SYMBOL& elsymb) const {
    if (index < 0 || index >= _elastic_symbols.size() ) {
      std::cerr << "Invalid index: " << index << std::endl;
      return ReturnStatus::ERR_INVALID_INDEX;
    }
    elsymb = _elastic_symbols[index];
    return ReturnStatus::SUCCESS;
  }

  AlphabetType Elastic_string::get_alphabet_type() const {
    return alphabet_type;
  }


  //////////////////////// private ////////////////////////

}// end namespace
