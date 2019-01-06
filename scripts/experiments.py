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
# Each file contains the (randomly generated) sequences of a given length (from  'text_size'); The number of sequences decided by 'num_seq_in_file'  .
# Each sequence of the specified length will have copies equal to size of 'k' (Here45).
# Each copy will have randomly distributed elastic-degenerate symbols equal to the given value from alpha.
# Each sybol will have a randomly chosen collection from the alphabet
#text_size = [1000, 2000, 4000, 8000, 16000, 32000, 64000]
#pattern_size = [8, 16, 32, 64]
#alpha = [10, 20, 40, 80] 
text_size = [10, 100]
pattern_size = [4,8]
alpha = [10]
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


def memory_usage_resource():
    import resource
    rusage_denom = 1024.
    if sys.platform == 'darwin':
        # OSX produces the output in different units
        rusage_denom = rusage_denom * rusage_denom
    mem = resource.getrusage(resource.RUSAGE_SELF).ru_maxrss / rusage_denom
    return mem


def collect_stats_old(o_file, stats_file):
    n = []
    d = []
    tm = []
    block_size = 5
    with open(o_file, "r") as f:
        mode = 0
        # 1st line seq name, next time, next n & k , next LPF array, next blank
        for line in f:
            if (mode == 0 or mode == 3 or mode == 4):
                # Ignore line                  
                dummy = line
            elif (mode ==1):
                tmf = re.findall("\d+\.\d+", line)
                tm.append(str(tmf[0]))
            else:
                seq_len, sd = line.split(' ')
                n.append(seq_len)
                d.append(sd)
            mode = (mode + 1) % block_size
    #num = num_seq_in_file * len(k)
    num=0
    for ind in range(num):
        stats_file.write(str(n[ind]) + param_separator + str(alpha[ind]) +
                         param_separator + tm[ind] + '\n')

def collect_stats(o_file):
    with open(o_file, "r") as f:
        # first line num of patterns ; ignore
        f.readline()

        # next line time
        line = f.readline()
        tmf = re.findall("\d+\.\d+", line)
        return str(tmf[0])       



def write_file(seq_file, seq_size, seq, a):
    d = seq_size / 10
    symb_pos = random.sample(range(seq_size), d)
    symb_pos_sorted = sorted(symb_pos)
    symb_pos_sorted.append(seq_size)
    nxt = 0
    
    #seq_file.write('>seq ' + str(seq_size) + '_' + str(a) + '\n')
    for ind in range(seq_size):
        if ind != symb_pos_sorted[nxt]:
            seq_file.write(seq[ind])
        else:
            empty_included = False
            sym_size = random.randint(2, a)
            seq_file.write('{')
            alleles = []
            for i in range(sym_size):
                allele_len = random.randint(0, max_len_in_els)
                allele = ''.join(random.choice(alphabet) for i in range(allele_len))
                if allele == '':
                    if empty_included:
                        i = i-1
                    else:
                        empty_included = True # first empty string included
                        allele = 'E'
                        alleles.append(allele)
                else:
                    alleles.append(allele)
            symb = ','.join(alleles)
            seq_file.write(symb)
            seq_file.write('}')
            nxt += 1
    
    seq_file.write('\n\n')


def onse_set(set_num):
    i_filename = FOLDER + DATA_FOLDER + str(set_num) + '/' + INPUT_FILE_NAME
    p_filename = FOLDER + DATA_FOLDER + str(set_num) + '/' + PATTERN_FILE_NAME
    o_filename = FOLDER + DATA_FOLDER + str(set_num) + '/' + OUTPUT_FILE_NAME
    

    # Generate Pattern Files
    #num_pat = len(pattern_size)
    for p_len in pattern_size:
        psuff = '.p' + str(p_len) + '.txt'
        pat_filename = p_filename + psuff
        pat_file = open(pat_filename, 'w')
        pattern = ''.join(random.choice(alphabet) for i in range(p_len))
        pat_file.write(pattern + '\n\n')
        pat_file.close()
    print("$$$$$$$$$$$$$$$$$$$$ PATTERN FILE GENERATION COMPLETE $$$$$$$$$$$$$$$$$$$$$$")

    # Generate Data Files
    num_files = len(text_size)
    for i in range(num_files):
        seq_size = text_size[i]
        seq = ''.join(random.choice(alphabet) for i in range(seq_size))
        for a in alpha:
            print("====== WRITING INPUT FILE ===== " + str(i))
            suff = '.n' + str(seq_size) + '.a' + str(a) + '.txt'
            seq_filename = i_filename + suff
            seq_file = open(seq_filename, 'w')            
            write_file(seq_file, seq_size, seq, a)
            seq_file.close()
    
    print("$$$$$$$$$$$$$$$$$$$$ INPUT FILE GENERATION COMPLETE $$$$$$$$$$$$$$$$$$$$$$")

    # Run the tool on the files
    for p_len in pattern_size:
        psuff = '.p' + str(p_len)
        pat_filename = p_filename + psuff + '.txt'
        
        for seq_size in text_size:
            for a in alpha:
                print("====== RUNNING PATTERN-INPUT FILE ===== " + str(p_len) + ' : ' + str(seq_size))
                suff = '.n' + str(seq_size) + '.a' + str(a)
                seq_filename = i_filename + suff + '.txt'
                outsuff = suff + psuff
                out_filename = o_filename + outsuff + '.txt'
                # Call the tool
                tool = './bin/eldes'
                cmd = tool + ' -a DNA -t ' + seq_filename + ' -p ' + pat_filename + ' -o ' + out_filename
                print('COMMAND: ' + cmd)
                comp = subprocess.Popen(cmd, shell=True)
                comp.wait()

    print("$$$$$$$$$$$$$$$$$$$$ FILE PROCESSING COMPLETE $$$$$$$$$$$$$$$$$$$$$$")

    # Analyse the output files
    for j in range(len(pattern_size)):
        p_len = pattern_size[j]
        psuff = '.p' + str(p_len)
        for i in range(len(text_size)):
            seq_size = text_size[i]
            for a in alpha:
                suff = '.n' + str(seq_size) + '.a' + str(a)
                outsuff = suff + psuff
                out_filename = o_filename + outsuff + '.txt'
                tm = float(collect_stats(out_filename))
                stats_dct[i][j] += tm
            


def main():
    for i in range(num_sets):
        cmd = 'mkdir ' + FOLDER + DATA_FOLDER + str(i+1)
        comp = subprocess.Popen(cmd, shell=True)
        comp.wait()

    for i in range(num_sets):
        print('********************************************* SET Number '+ str(i) +' ************************************' )
        onse_set(i+1)
        print('************************ done\n')

    sf = open(FOLDER + STATS_FILE_NAME+'.stats', 'w')        
    sf.write(param_separator.join(stats_param))
    # Analyse the output files
    for j in range(len(pattern_size)):
        p_len = pattern_size[j]
        sf.write('\n')
        for i in range(len(text_size)):            
            seq_size = text_size[i]
            stats = str(p_len) + param_separator + str(seq_size) + param_separator                
            tm = stats_dct[i][j]
            tm = tm/num_sets
            stats += str(tm)
            sf.write(stats)
            sf.write('\n')
    sf.close()

main()
