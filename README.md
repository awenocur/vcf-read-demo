# vcf-read-demo
This is a demonstration of parsing a VCF file using htslib. It works only on diploid VCF files; polyploid ones will result in an exception being thrown.


This has been tested using GCC 11 on GNU/Linux. Simply build using the Makefile:

```
cd vcf-read-demo
make
```
