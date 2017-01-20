#include <limits.h>
#include <vector>
#include <string>
#include <iterator>
#include "Parser.hpp"
#include "utilDefs.hpp"
#include "globalDefs.hpp"
#include "Elastic_string.hpp"
#include "gtest/gtest.h"

void revert (std::string& str) {
std::transform (str.begin(), str.end(), str.begin(), eldes::reverse_map);
}


TEST(validInputTest, SingleSeed) {
  eldes::Elastic_string els;
  eldes::Parser::parse_elastic(eldes::AlphabetType::DNA, "test_files/seed_only.txt", els);
  eldes::SEED seed;
  els.get_seed(0, seed);
  revert(seed);
  EXPECT_EQ(seed, "ATTATG");
}

TEST(validInputTest, MultipleSeeds2) {
  // Two seeds.
  eldes::Elastic_string els;
  eldes::Parser::parse_elastic(eldes::AlphabetType::DNA, "test_files/positive_seed_2.txt", els);
  int ind=0;
  for (auto str: {"ATTG", "CTCTCT"}) {
	eldes::SEED seed;
	els.get_seed(ind++, seed);
    revert(seed);
	EXPECT_EQ(seed, str);
  }
  ind = 0; 
  eldes::ELASTIC_SYMBOL elsymb;
  els.get_elastic_symbol(0, elsymb);
  for (auto str: {"ATTG", "ATTGG"}) {
	revert(elsymb[ind]);
	EXPECT_EQ(elsymb[ind], str);
    ind++;
  }
}

TEST(validInputTest, MultipleSeeds3) {
  // Three seeds
  eldes::Elastic_string els;
  eldes::Parser::parse_elastic(eldes::AlphabetType::DNA,"test_files/positive_seed_3.txt", els);
  int ind=0;
  for (auto str: {"ATTG", "CTCTCT", "CCCCTT"}) {
	eldes::SEED seed;
	els.get_seed(ind++, seed);
    revert(seed);
	EXPECT_EQ(seed, str);
  }
  ind = 0; 
  eldes::ELASTIC_SYMBOL elsymb;
  els.get_elastic_symbol(0, elsymb);
  for (auto str: {"ATTG", "ATTGG", ""}) {
	revert(elsymb[ind]);
	EXPECT_EQ(elsymb[ind], str);
    ind++;
  }
  ind = 0; 
  els.get_elastic_symbol(1, elsymb);
  for (auto str: {"ATTC", "GTTATT", "ATATAT"}) {
	revert(elsymb[ind]);
	EXPECT_EQ(elsymb[ind], str);
    ind++;
  }
}

TEST(validInputTest, EmptySeedsandAlleles) {
  // Three seeds
  eldes::Elastic_string els;
  eldes::Parser::parse_elastic(eldes::AlphabetType::DNA,"test_files/empty_seed_alleles.txt", els);
  int ind=0;
  for (auto str: {"", "CTCTCT", ""}) {
	eldes::SEED seed;
	els.get_seed(ind++, seed);
    revert(seed);
	EXPECT_EQ(seed, str);
  }
  ind = 0; 
  eldes::ELASTIC_SYMBOL elsymb;
  els.get_elastic_symbol(0, elsymb);
  for (auto str: {"ATTG", "ATTGG"}) {
	revert(elsymb[ind]);
	EXPECT_EQ(elsymb[ind], str);
    ind++;
  }
  ind = 0; 
  els.get_elastic_symbol(1, elsymb);
  for (auto str: {"ATTC", "", "ATATAT"}) {
	revert(elsymb[ind]);
	EXPECT_EQ(elsymb[ind], str);
    ind++;
  }
}


TEST(InvalidInput, WrongInput) {
  eldes::Elastic_string els;
  // empty file
  eldes::ReturnStatus rs = eldes::Parser::parse_elastic(eldes::AlphabetType::DNA,"test_files/empty.txt", els);
  EXPECT_EQ(rs, eldes::ReturnStatus::ERR_INVALID_INPUT);

  // invalid symbol
  rs = eldes::Parser::parse_elastic(eldes::AlphabetType::DNA,"test_files/invalid_symbol.txt", els);
  EXPECT_EQ(rs, eldes::ReturnStatus::ERR_INVALID_INPUT);

  // invalid first marker
  rs = eldes::Parser::parse_elastic(eldes::AlphabetType::DNA,"test_files/invalid_first_marker.txt", els);
  EXPECT_EQ(rs, eldes::ReturnStatus::ERR_INVALID_INPUT);

  // invalid ending first marker
  rs = eldes::Parser::parse_elastic(eldes::AlphabetType::DNA,"test_files/invalid_ending_marker.txt", els);
  EXPECT_EQ(rs, eldes::ReturnStatus::ERR_INVALID_INPUT);


}



