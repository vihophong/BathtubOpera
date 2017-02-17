# BathtubOpera
AIDA unpacker, merger  and decay builder for EURICA

#Requirements
The package was tested in a Centos 7.3 computer with gcc 4.8.5 and CERN-root 5.34.36.

It may also worked with lower CERN-root version

In addition, cmake version > 3.0 is required. To upgrade cmake version in Centos 7: sudo yum install cmake3

##  To Compile
cd BathtubOpera/build

cmake ../

(or cmake3 ../)

make


## Run event builder and merger from sample data

Put AIDA, BiGRIPS and EURICA data into the directory: BathtubOpera/build/sampleaida, BathtubOpera/build/sampleeurica  and BathtubOpera/build/samplebigrip

Execute, for example:

./buildionbeta.sh 43 0 1 sampleeurica/eurica_run2027.root samplebigrips/bigrips_run2132.root outionbeta.root

which corresponds to:

./buildionbeta.sh aida_run_number aida_subrun_number_lower aida_subrun_number_upper path_to_eurica path_to_bigrips path_to_outputfile

here I assumed that two files AIDA43_0 and AIDA43_1 are copied into the directory BathtubOpera/build/sampleaida


## Run decay builder (merging between ion and beta) from the previous output file

Assumming the previously produced file is "outionbeta.root", execute:

./aidabuilddecay outionbeta.root outdecay.root

where

./aidabuilddecay input_file output_file


## Tips

Run 

./buildionbeta.sh

to display help.

look inside the file buildionbeta.sh to understand how this shell script works. Basically its task is to create a list of AIDA files to be process, then run the compiled program called "ionbetanew".

If one execute the program:

./ionbetanew 

Then it will display:

AIDA event builder

use ./ionbetanew with following flags:

        [-a    <char*    >: AIDA input list of files]

        [-eu   <char*    >: EURICA input file]

        [-br   <char*    >: BIGRIPS input file]

        [-o    <char*    >: output file]

        [-wd   <long long>: Fast Discriminator Scan window (default: 0 i.e no scan for fast discrimination)]

        [-v    <int      >: verbose level]

        [-map  <char*    >: mapping file]

        [-cal  <char*    >: calibration file]

        [-thr  <char*    >: threshold file]

        [-f    <int      >: fill data or not: 1 fill data 0 no fill (default: fill data)]

        [-ecut <char*    >: specify energy cut file]

        [-gz   <int      >: input data from gz file: 1 enable 0 disable (default: disable)]

No AIDA input list of files given 


The configuration files are located in BathtubOpera/build/config

which contains the mapping table, calibration table, threshold table and DSSD energy cut table.


