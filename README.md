# Algorithms-in-c++-
Bioinformatics Algorithims

Algorithms being implemented are Quicksort, Needleman-Wunsch, K-means clustering, PSSM, and a Gibbs Sampler


Files for sorting are: sample100k.fastq sample10k.fastq sample1k.fastq
//sorting based upong sequence

g++ try.cpp && time ./a.out sample1m.fastq > test.txt


Files to be aligned: RpoB-B.subtilis.fasta RpoB-E.coli.fasta
//Run Needleman_Wunsch Alignment with match score, mismatch score, and gap score

g++ jack_NW.cpp -o ts
./ts RpoB-B.subtilis.fasta RpoB-E.coli.fasta 1 -3 -4

File to be clustered bacteria.txt
//K means clustering algorithm with ranging centroid possibilites from min to max number

g++ Jack_Clustering.cpp -o cls
 ./cls bacteria.txt 3 7

Files to have PSSM ran on are FurR.txt and ecoK12-MG1655.fasta
//PSSM algorithm with minimum scores shown

g++ ustanik_pssm.cpp -o ps
./ps FruR.txt ecoK12-MG1655.fasta 15


Files to have Gibbs sampler ran on are E.coliRpoN-sequences-16-100nt.fasta E.coliRpoN-sequences-6-300nt.fasta
//Gibbs sampler program with listed initial start size

g++  ustanik_gibbs.cpp -o gb
./gb E.coliRpoN-sequences-16-100nt.fasta 39
