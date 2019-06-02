# Query Generator

## Run

example:

    mkdir objs
    make clean
    make
    ./test.sh

## Command Line Arguments

example:(in test.sh)

    ./run.exe data/num.nt test.txt gen_query
data path, query pattern path, output directory

## Query pattern 

example:(in test.txt)

    3 2 2
    4 3 1
    4 4 1
    -1

for each line:

vertex num, edge num, how many sql files to genenrate
