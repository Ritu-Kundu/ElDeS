import string
import random
import re
import math
import os
import subprocess
import time
import sys
######### Settings of the experiments #############
alphabet = ['A', 'C', 'G', 'T']
pat_alphabet = ['A', 'C', 'G', 'T']
txt_alphabet = ['C', 'G', 'T']
# Each file contains the (randomly generated) sequences of a given length (from  'text_size'); The number of sequences decided by 'num_seq_in_file'  .
# Each sequence of the specified length will have copies equal to size of 'k' (Here45).
# Each copy will have randomly distributed elastic-degenerate symbols equal to the given value from alpha.
# Each sybol will have a randomly chosen collection from the alphabet
#text_size = [1000, 2000, 4000, 8000, 16000, 32000, 64000]
#pattern_size = [8, 16, 32, 64]
#alpha = [10, 20, 40, 80] 
text_size = [100]
pattern_size = [10]
alpha = 10
deg_symb = 10 # in percent
max_len_in_els = 10 
num_sets = 2
param_separator = '\t'
###################################################

FOLDER = './experiments/'
DATA_FOLDER = 'data/'
INPUT_FILE_NAME = 'input'
PATTERN_FILE_NAME = 'pattern'
OUTPUT_FILE_NAME = 'output'
STATS_FILE_NAME = 'stats.txt'

stats_param = ['m', 'n', 'time']

###################################################
# stats dictionary: rows= seq_size, columns = pattern_size
stats_dct = [[0,0],[0,0]]

occs = []
def generate_alleles(num_allele, pat):
    alleles = []
    for i in range(num_allele):
        allele_len = random.randint(1, max_len_in_els)
        allele = ''.join(random.choice(txt_alphabet) for i in range(allele_len))
        if (allele.startswith(pat) or pat.startswith(allele)):
            i = i-1
        else:
            alleles.append(allele)
    return alleles
# Seq size should be > 100 and pattern size >10
def write_sequence(seq_file, seq_size, pattern, a):
    p_len = len(pattern)
    #seq_file.write('>seq ' + str(seq_size) + '_' + str(a) + '\n')
    # There are 5 occurrences of the pattern inserted:
    #  - One, in a seed (length of text increses by the |p|)
    #  - Two, entirely in one symbol (to check it is reported just once)
    #  - One, entirely in one symbol
    #  - One, spanning only one symbol (starting in seed)
    #  - One, spanning three symbols starting in seed (one symbol contains empty)
    #  - One, starts in a symbol and ends in the follwing symbol
    index = 0
    seed1 = ''.join(random.choice(txt_alphabet) for i in range(3))
    index += 3
    seed1 +=  pattern
    occs.append((index,index+p_len-1))
    index += p_len
    seed1 +=''.join(random.choice(txt_alphabet) for i in range(4))
    index += 4
    seq_file.write(seed1)
    print("Occ1  (seed only) done: " + seed1)
    
    symb1 = generate_alleles(3, pattern)
    # pattern as entire constituent
    symb1.append(pattern)
    # patterna s a factor of constituent
    symb1.append(seed1)
    occs.append((index,index))
    index += 1
    seq_file.write('{' + ','.join(symb1) + '}')
    print("Occ2 (two in one symbol) done: " + ','.join(symb1))
    
    tmp_seed = ''.join(random.choice(txt_alphabet) for i in range(p_len+1))
    index += len(tmp_seed)
    seq_file.write(tmp_seed)
    print("Added sperator seed: " + tmp_seed)
    
    symb2 = generate_alleles(4, pattern)
    # pattern as entire constituent
    symb2.append(pattern)
    symb2.append('E')
    occs.append((index,index))
    index += 1
    seq_file.write('{' + ','.join(symb2) + '}')
    print("Occ3 (one in one symbol with e as constituent) done: " + ','.join(symb2))
    
    break_pos = random.sample(range(2,p_len-1), 2)
    break_pos_sorted = sorted(break_pos)
    b1 = break_pos_sorted[0]
    b2 = break_pos_sorted[1]
    seed2 = pattern[:b1]
    start = index
    index += len(seed2)
    symb3= generate_alleles(3, pattern[b1:b2])
    symb3.append(pattern[b1:b2])
    index += 1
    seed3 = pattern[b2:]
    index += len(seed3)
    occs.append((start,index-1))
    seq_file.write(seed2+'{' + ','.join(symb3) + '}'+seed3)
    print("Occ4 (spanning one symbol) done: " + seed2+'{' + ','.join(symb3) + '}'+seed3)
    
    break_pos5 = random.sample(range(2,p_len-1), 5)
    break_pos_sorted5 = sorted(break_pos5)
    b51 = break_pos_sorted5[0]
    b52 = break_pos_sorted5[1]
    b53 = break_pos_sorted5[2]
    b54 = break_pos_sorted5[3]
    b55 = break_pos_sorted5[4]
    seed51 = pattern[:b51]
    start5 = index
    index += len(seed51)
    symb51= generate_alleles(3, pattern[b51:b52])
    symb51.append(pattern[b51:b52])
    index += 1
    
    seed52 = pattern[b52:b53]
    index += len(seed52)
    symb52= generate_alleles(3, pattern[b53:b54])
    symb52.append('E')
    index += 1
    
    seed53 = pattern[b53:b54]
    index += len(seed53)
    symb53= generate_alleles(4, pattern[b54:b55])
    symb53.append(pattern[b54:b55])
    index += 1
    
    seed54 = pattern[b55:]
    index += len(seed54)
    
    occs.append((start5,index-1))
    temp_seq = seed51+'{' + ','.join(symb51) + '}'+seed52 + '{' + ','.join(symb52) + '}'+ seed53 + '{' + ','.join(symb53) + '}' + seed54
    seq_file.write(temp_seq)
    print("Occ5 (spanning three symbols, middle with E) done: " + temp_seq)
    
    break_pos6 = random.sample(range(2,p_len-1), 2)
    break_pos_sorted6 = sorted(break_pos6)
    b61 = break_pos_sorted6[0]
    b62 = break_pos_sorted6[1]
    start = index
    symb61= generate_alleles(3, pattern[:b61])
    symb61.append(pattern[:b61])
    index += 1
    seed61 = pattern[b61:b62]    
    index += len(seed61)
    symb62= generate_alleles(4, pattern[b62:])
    symb62.append(pattern[b62:])
    index += 1
    occs.append((start,index-1))
    seq_file.write('{' + ','.join(symb61) + '}'+seed61+'{' + ','.join(symb62) + '}')
    print("Occ6 (starting and ending in symbols) done: " + '{' + ','.join(symb61) + '}'+seed61+'{' + ','.join(symb62) + '}')
    
    symb7= generate_alleles(4, pattern)
    symb7.append('E')
    index += 1
    seq_file.write('{' + ','.join(symb7) + '}')
    print("Extra symbol: " + '{' + ','.join(symb7) + '}')
    print ("Index is %d "%(index))
        
    d = math.floor(seq_size / 10)
    d = d-9
    symb_pos = random.sample(range(index,seq_size), d)
    symb_pos_sorted = sorted(symb_pos)
    symb_pos_sorted.append(seq_size)
    nxt = 0
    seq = ''.join(random.choice(txt_alphabet) for i in range(seq_size))
    
    for ind in range(index, seq_size):
        if ind != symb_pos_sorted[nxt]:
            seq_file.write(seq[ind])
        else:            
            sym_size = random.randint(2, a)
            al = generate_alleles(sym_size,pattern)            
            symb = ','.join(al)
            seq_file.write('{'+','.join(al)+'}')
            nxt += 1
    
    seq_file.write('\n\n')

def onse_set(p_len, seq_size):
    i_filename = FOLDER + DATA_FOLDER +  INPUT_FILE_NAME
    p_filename = FOLDER + DATA_FOLDER +  PATTERN_FILE_NAME
    o_filename = FOLDER + DATA_FOLDER +  OUTPUT_FILE_NAME
    

    # Generate Pattern File
    # First letter of the pattern is A (which is not in the text)
    psuff = '.p' + str(p_len) + '.txt'
    pat_filename = p_filename + psuff
    pat_file = open(pat_filename, 'w')
    pattern = 'A' + ''.join(random.choice(pat_alphabet) for i in range(p_len-2)) + 'A'
    print('Pattern: %s'%(pattern))
    pat_file.write(pattern + '\n\n')
    pat_file.close()
    print("$$$$$$$$$$$$$$$$$$$$ PATTERN FILE GENERATION COMPLETE $$$$$$$$$$$$$$$$$$$$$$")

    # Generate Data File
    print("====== WRITING INPUT FILE ===== ")
    suff = '.n' + str(seq_size) + '.a' + str(alpha) + '.txt'
    seq_filename = i_filename + suff
    seq_file = open(seq_filename, 'w')            
    write_sequence(seq_file, seq_size, pattern, alpha)
    seq_file.close()    
    print("$$$$$$$$$$$$$$$$$$$$ INPUT FILE GENERATION COMPLETE $$$$$$$$$$$$$$$$$$$$$$")

    # Run the tool on the files

    print("====== RUNNING PATTERN-INPUT FILE ===== " + str(p_len) + ' : ' + str(seq_size))
    outsuff = suff + psuff
    out_filename = o_filename + outsuff
    # Call the tool
    tool = './bin/eldes'
    cmd = tool + ' -a DNA -t ' + seq_filename + ' -p ' + pat_filename + ' -o ' + out_filename
    print('COMMAND: ' + cmd)
    comp = subprocess.Popen(cmd, shell=True)
    comp.wait()

    print("$$$$$$$$$$$$$$$$$$$$ FILE PROCESSING COMPLETE $$$$$$$$$$$$$$$$$$$$$$")

    # Analyse (compare) the output files
    obs_occs = []
    with open(out_filename, 'r') as f:
        num_patterns = f.readline().split()
        time_str = f.readline().split()
        for line in f:
            if (line.split() !=''):
                if (line.startswith('>')):
                    p_name=line
                    # ignore
                elif (line.startswith('#')):
                    num_occs = line.split()[0]
                    print(num_occs)
                else:
                    occ_pairs = line.split(';')
                    occ_pairs = occ_pairs[:-1]
                    for o in occ_pairs:
                        o = o.split()[0]
                        s,e = o.split(',')
                        st = int(s[1:])
                        en = int(e[:-1])
                        obs_occs.append((st,en))
    if (obs_occs == occs):
        print('SUCCESS.')
    else:
        print('FAIL!!!!')
        print(occs)
        print(obs_occs)
        
def main():
    for t in text_size:
        for p in pattern_size:
            print("TESTING: t:%d p:%d"%(t,p))
            onse_set(p, t)
main()