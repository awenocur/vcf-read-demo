build: demo
vcf-util.o: vcf-util.hpp vcf-util.cpp
	c++ -c -o vcf-util.o -ggdb -O0 vcf-util.cpp
demo: vcf-util.o vcf-util.hpp demo.cpp
	c++ -o demo demo.cpp vcf-util.o -lhts -ggdb -O0
