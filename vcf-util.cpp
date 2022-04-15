#include "vcf-util.hpp"
#include <htslib/hts.h>

void VCFFile::open(std::filesystem::path path)
{
  done = false;
  this->path = path;
  if(content != NULL)
    {
      hts_close(content);
      bcf_hdr_destroy(header);
    }
  content = hts_open(&static_cast<std::string>(path)[0], "r");
  header = bcf_hdr_read(content);
}

VCFFile::VCFFile():content(NULL), header(NULL)
{
  row = bcf_init();
}

VCFFile::~VCFFile()
{
  if(content != NULL)
    {
      hts_close(content);
      bcf_hdr_destroy(header);
    }
  bcf_destroy(row);
}

Variant VCFFile::computeRow()
{
  bcf_unpack(row, BCF_UN_ALL);

  uint32_t numSamples = bcf_hdr_nsamples(header);
  int32_t *genotypeArray = NULL;
  int32_t genotypeCardinality = 0;
  bcf_get_genotypes(header, row, &genotypeArray, &genotypeCardinality);
  unsigned int homref = 0, homalt = 0, missing = 0;
  unsigned int het = 0;
  for(unsigned int i = 0; i < numSamples; i++)
    {
      assert(genotypeCardinality == 2*numSamples);
      unsigned int allele1 = bcf_gt_allele(genotypeArray[2*i]);
      unsigned int allele2 = bcf_gt_allele(genotypeArray[2*i+1]);
      if(bcf_gt_is_missing(genotypeArray[2*i]) || bcf_gt_is_missing(genotypeArray[2*i+1]))
	missing++;
      else
	if(allele1 != allele2) het++;
      else if(allele1 == 0) homref++;
      else homalt++;
    }
  return Variant(bcf_hdr_id2name(header, row->rid), row->pos, homref, homalt, het, missing);
}

int VCFFile::read()
{
  return bcf_read(content, header, row);
}

VCFFile::iterator VCFFile::begin()
{
  if(read() == -1) done = true;
  iterator result;
  result.isEnd = false;
  result.file = this;
  return result;
}

VCFFile::iterator VCFFile::end()
{
  iterator result;
  result.file = this;
  result.isEnd = true;
  return result;
}

VCFFile::iterator &VCFFile::iterator::operator ++ ()
{
  if(file->read() == -1) file->done = true;
  return *this;
}

bool VCFFile::iterator::operator == (VCFFile::iterator &other)
{
  return file->done && other.isEnd;
}

bool VCFFile::iterator::operator != (VCFFile::iterator &other)
{
  return !file->done || !other.isEnd;
}

const Variant VCFFile::iterator::operator *()
{
  assert(!isEnd);
  return file->computeRow();
}

Variant::Variant(std::string chr, hts_pos_t pos, unsigned int homref, unsigned int homalt, unsigned int het, unsigned int missing):chr(chr), pos(pos), homref(homref), homalt(homalt), het(het), missing(missing)
{}
