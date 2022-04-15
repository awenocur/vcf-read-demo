#include <iostream>
#include <fstream>
#include "vcf-util.hpp"

using namespace std;

int main(int argc, const char** argv)
{
  if(argc != 3)
    {
      cerr << "Usage: " << argv[0] << " <input VCF> <output TSV>\n";
      exit(1);
    }
  filesystem::path inputPath = argv[1];
  filesystem::path outputPath = argv[2];
  if(!filesystem::is_regular_file(inputPath))
    {
      cerr << "input VCF is not a regular file\n";
      exit(1);
    }
  ofstream output;
  output.open(outputPath);
  if(output.bad() | output.fail())
    {
      cerr << "could not open output file\n";
      exit(1);
    }
  
  VCFFile fileToCheck;
  fileToCheck.open(inputPath);
  output << "CHROM\tPOS\tMISSING\tHOMREF\tHOMALT\tHET\n";
  for(auto variant: fileToCheck)
    {
      output << variant.chr << "\t" << variant.pos << "\t" << variant.missing << "\t" << variant.homref << "\t" << variant.homalt << "\t" << variant.het << "\n";
    }
  return 0;
}
