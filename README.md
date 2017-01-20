#ElDeS : Pattern Matching in Elastic-Degenerate strings
=======================================================

*ElDeS* is a tool that finds out occurrences of degenerate patterns in an elastic-degenerate text.

>A *degenerate string* which is defined by the existence of one or more positions that are represented by sets of symbols from
>an alphabet Σ, unlike an accurate or certain (standard) string characterised by a single symbol at each position.
>For instance, {a,b} a c {b,c} a {b,c} is a degenerate string of length 6 over Σ = {a,b,c}.

>An *elastic-degenerate string* can been seen as an ordered collection of k > 1 seeds (substrings/subpatterns) interleaved
>by elastic-degenerate symbols such that each elastic-degenerate symbol corresponds to a set of two or more variable length strings.
> bc {ab, aab, aca} ca {abcab, cba} bb is an example of an elastic-degenerate string over Σ = {a,b,c}.

The tool is based on the terminology and the algorithm described in the paper titled
[**Efficient Pattern Matching in Elastic-Degenerate Strings**](https://arxiv.org/abs/1610.08111)
by *Costas S. Iliopoulos, Ritu Kundu, Solon P. Pissis*

To compile ElDeS, please follow the instructions given in file `INSTALL.md`


## Usage of the tool: 
./bin/eldes <options>
with the following mandatory options:
- -a, --alphabet <str>				'DNA' for nucleotide  sequences or 'PROT' for protein  sequences.
- -t, --text-input-file <str>		Input file  name for text (Elastic-degenerate string)
- -p, --pattern-input-file <str>	Input file  name for (degenerate) patterns.
- -o, --output-file	<str>			Output filename.

 **Example:**  ./bin/eldes -a DNA -t ./sample/sampleText.txt -p ./sample/samplePattern.txt -o ./sample/sampleOutput.txt

Here, the elastic-degenerate string/text and degenerate patterns are given in files "sampleText.txt" and "samplePattern.txt" which are in subfolder "sample" of current folder. 
Occurrences will be calculated and result will be written in the file "sampleOutput.txt" in subfolder "result" of current folder.

## File-formats:
- Alphabet can be single letter codes representing bases in genomic sequences (for DNA) or amino acids in protiens (for PROT).

- Pattern file is expected to be in a valid [FASTA format](https://en.wikipedia.org/wiki/FASTA_format).
 * Valid DNA letters: ACGTUNRDHKMSWYVB (irrespective of case)
 * Valid Prot letters: ABCDEFGHIJKLMNPQRSTUVWXYZ (irrespective of case)

- Elastic-degenerate string/text is required to be in PRG (Population Reference Genome) format. Details of the format can be read in the paper: *Maciuca, S., del Ojo Elias, C., McVean, G., Iqbal, Z.: [A Natural Encoding of Genetic Variation in a Burrows-Wheeler Transform to Enable Mapping and Genome Inference](http://dx.doi.org/10.1007/978-3-319-43681-4_18), pp. 222–233. Springer International Publishing, Cham (2016) *
Broadly - 
 * An Elastic-degenerate string is an ordered collection of k > 1 seeds (substrings) interleaved by elastic-degenerate symbols such that each elastic-degenerate. More formally, an elastic-degenerate string, over a given alphabet Σ, is a sequence S_1 ξ_1 S_2 ξ_2 S_3 . . S _{k−1} ξ_{k−1} S_k , where S_i , 1 ≤ i ≤ k is a seed and ξ_i , 1 ≤ i ≤ k − 1 is an elastic-degenerate symbol. An elastic-degenerate symbol
ξ, over a given alphabet Σ, is a non-empty set of strings over Σ (i.e ξ ⊂ Σ^∗ and ξ != ∅). 
 * ACCT { ACC , AA , ATT } CCCTA { AC , A } CC  is an example of an elastic-degenerate string with 3 seeds -- s_1=ACCT, S_2=CCCTA, S_3=CC -- and two elastic symbols -- ξ_1 = { ACC , AA , ATT }, ξ_2={ AC , A }.
 * Seed can be empty; An elastic-degenerate symbol can not be empty but can contain an empty string; An elastic-degenerate symbol must contain at least 2 strings.
 * An elastic symbol is marked by a unique odd number. The odd number will mark its boundaries (beginning and end). The strings within this symbol are delimited by a unique even number. The above example in PRGs encoding will look like: ACCT 5 ACC 6 AA 6 ATT 5 CCCTA 7 AC 8 A 7 CC
 * White space characters between letters and between a letter and a number are allowed. White-space characters between numbers represent an empty string between them.
 * File can not be empty.
 * Valid DNA letters: ACGTU (irrespective of case)
 * Valid Prot letters: ACDEFGHIKLMNPQRSTUVWY (irrespective of case)

- Output file is in the following format:
 * It writes the number of patterns searched.
 * It also gives the time used for calculation (after input file has been read in memory and before writing the results).
 * After that, corresponding to each pattern, there is a block (two blocks are separated by an empty line): 
  * The first line in the block begins with a '>' followed by the identifier(FASTA format) of the pattern.
  * The next line starts with a '#' followed by the number of occurrences of the pattern found.
  * The next line reports the occurrence indices delimited bt a ' ; '.
 * An occurrence is shown as a pair of two numbers (x, y) where x is the starting position of the occurrence and y is its ending position.
   * A *position* is an index (zero-based) of a letter (considering an elastic-degenerate symbol to occupy only one position).
   * For example: a pattern "ACC" in the above elastic-degenerate string occurs at positions: (0,2) ; (4,4) ; (4,6) ; (10,11) ; (10,12) ; 
   * Leading and/or trailing empty seeds or strings in an elastic-symbol, of an occurrence, are ignored and the occurrence is reported to start/end in the appropriate non-empty elastic-symbol/seed (see the paper for more details).

## Highlights:

A few highlights of the tool are:
- It outputs starting as well as ending positions of an occurrence.
- It can process multiple patterns given as a batch, i.e. the pre-processing stage will take less time than that taken for the same patterns individually.
- It can handle degenerate/indeterminate stringd (Sequences with SNPs). 


## External Libraries:

 * For construction of Suffix-Array, [Jesper Larsson's](http://www.itu.dk/people/jesl/) [implementation](http://www.larsson.dogma.net/qsufsort.c) of suffix array sorting on integer-alphabets (description of [Larsson and Sadakane](http://www.sciencedirect.com/science/article/pii/S0304397507005257)).
 * For answering rmq (to find longest common prefix), following library has been used:
  + [sdsl](https://github.com/simongog/sdsl-lite)
 * For testing [googletest](https://github.com/google/googletest) framework has been used.

## Experiments:
- "Experiments" folder contains the text(of about 4MB), the patterns, and the corresponding output.
- The text was randomly generated for the experiments (about 1000 positions long). 
 + 10% of the text was made to have Elastic-Degenerate symbols.
 + Each of these symbols consisted of 10 strings.
 + Each of the string in an elastic-degenerate symbol was at most 10 characters long.
- Format of the elastic-degenerate string (i.e. the text) was different from the PRG format (corresponding parser is within the same folder).
 + Each elastic-degenerate symbol began with '{' and ended with '}'.
 + Strings within an elastic-degenerate symbol were separated by a ','. An empty such string was represented by the letter 'E'.
- 10 patterns of varying length were randomly generated.
- Each pattern was searched in the text individually.

