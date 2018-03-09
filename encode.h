#pragma once
#include "compress.h"

class Encode : public Compress
{
	int low, high;
	int opposite_bits;
	int buffer;
	int	bits_in_buf;

	ifstream in;
	ofstream out;
public:
	Encode(void);
	~Encode(void);
	
	void write_bit( int bit);
	void output_bits(int bit);
	void end_encoding(void);
	void encode_symbol(int symbol);
	void encode(char *infile, char *outfile);

};

