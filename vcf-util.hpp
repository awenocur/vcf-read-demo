#include <string>
#include <htslib/vcf.h>
#include <filesystem>

class Variant
{   
public:
  const std::string chr;
  const hts_pos_t pos;
  const unsigned int homref;
  const unsigned int homalt;
  const unsigned int het;
  const unsigned int missing;
  Variant(std::string, hts_pos_t, unsigned int, unsigned int, unsigned int, unsigned int);
};

/** wrapper for HTSLib VCF file */
class VCFFile
{
  bool done = false;
  htsFile *content;
  bcf_hdr_t *header;
  bcf1_t *row;
  std::filesystem::path path;
  int read();
  Variant computeRow();
public:
  VCFFile();
  ~VCFFile();
  void open(std::filesystem::path path);
  class iterator;
  iterator begin();
  iterator end();
  class iterator
  {
    friend iterator VCFFile::begin();
    friend iterator VCFFile::end();
    bool isEnd;
    VCFFile *file;
  public:
    iterator next();
    const Variant operator *();
    bool operator == (iterator &other);
    bool operator != (iterator &other);
    VCFFile::iterator &operator ++();
  };
};
