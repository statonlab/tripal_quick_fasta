[![Build Status](https://travis-ci.org/statonlab/tripal_quick_fasta.svg?branch=master)](https://travis-ci.org/statonlab/tripal_quick_fasta)


 This module's development is halted as a beta.  As a pre-release, it only creates one type of FASTA file: the sequences are all **polypeptide** features, with the headers being the uniquename of the parent **mRNA** or **mRNA_contig**.


### Module Goals
If we did develop this module, this is a feature list:

* Download all sequences from a Tripal site in a single FASTA file.
* Download all sequences from a Tripal site, one file per organism
* Specify which type of feature to download (mRNA, proteins)
* specify which feature should bear the name based on the feature relationship (Download the proteins, but named as the mRNA)
