# Overview
Scallop is an accurate reference-based transcript assembler. Scallop features
its high accuracy in assembling multi-exon transcripts as well as lowly
expressed transcripts. Scallop achieves this improvement through a novel
algorithm that can be proved preserving all phasing paths from paired-end reads,
while also achieves both transcripts parsimony and coverage deviation minimization.

# Release
Latest release, including both binary and source code, is [here](https://github.com/Kingsford-Group/scallop/releases/tag/v0.9.6).

# Installation
To install Scallop from the source code, you need to first download/compile 
htslib, Boost and Clp, setup the corresponding environmental variables,
and then compile the source code of Scallop.

## Install htslib
Download htslib [(license)](https://github.com/samtools/htslib/blob/develop/LICENSE)
from (http://www.htslib.org/) with version 1.2 or higher.
Compile it to generate the htslib file `libhts.a`. 
Set environment variable `HTSLIB` to indicate the directory of `libhts.a`.
For example, for Unix platforms, do the following:
```
export HTSLIB="/directory/to/your/htslib/htslib-1.2.1"
```

## Install Boost
Download Boost [(license)](http://www.boost.org/LICENSE_1_0.txt)
from (http://www.boost.org).
Uncompress it somewhere (compiling and installing are not necessary).
Set environment variable `BOOST_HOME` to indicate the directory of Boost.
For example, for Unix platforms, do the following:
```
export BOOST_HOME="/directory/to/your/boost/boost_1_60_0"
```

## Install Clp
Download Clp [(license)](https://opensource.org/licenses/eclipse-1.0)
from (https://projects.coin-or.org/Clp), compile and install it.
Set environment variable `CLP_HOME` to indicate the installed directory.
For example, for Unix platforms, do the following:
```
export CLP_HOME="/directory/to/your/clp/install"
```

## Compile Scallop
The compilation of `scallop` requires `automake` package.
If `automake` has not been installed, on linux platform, do the following:
```
sudo apt-get install automake
```
Then run the script `build.sh`, which will generate the executable file `src/src/scallop`.


# Usage

The usage of `scallop` is:
```
./scallop -i <input.bam> -o <output.gtf> [options]
```

The `input.bam` is the read alignment file generated by some RNA-seq aligner, (for example, TopHat2, STAR, or HISAT2).
Make sure that it is sorted; otherwise run `samtools` to sort it:
```
samtools sort input.bam > input.sort.bam
```

The reconstructed transcripts shall be written as gtf format into `output.gtf`.

Scallop support the following parameters:

Parameters | Default Value | Description
------------ | ------------- | ----------
 --help  |  | print usage of Scallop and exit
 --version | | print version of Scallop and exit
 --verbose | 1 | chosen from {0, 1, 2}; 0: quiet; 1: one line for each splice graph; 2: details of graph decomposition
 --library_type               | unstranded | chosen from {first, second, unstranded}
 --min_transcript_coverage    | 1.01 | the minimum coverage required to output a multi-exon transcript
  --min_single_exon_coverage   | 20 | the minimum coverage required to output a single-exon transcript
 --min_exon_length            | 50 | the minimum length of a transcript is: --min_transcript_length <br> + --min_exon_length * num-of-exons
 --min_transcript_length      |250 | the minimum length of a transcript is: --min_transcript_length <br> + --min_exon_length * num-of-exons
 --min_mapping_quality        | 1 | ignore reads with mapping quality less than this value
 --min_bundle_gap             |50 | the minimum distances required to start a new bundle
 --min_num_hits_in_bundle     | 20 | the minimum number of reads required in a bundle
 --min_flank_length           | 3 | the minimum match length required in each side for a spliced read
 --min_splice_bundary_hits    | 1 | the minimum number of spliced reads required to support a junction
