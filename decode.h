#pragma once
#include "compress.h"

class Decode : public Compress
{
	long low, high;
	long value;

	int buffer;
	int	bits_in_buf;
	int eof_reads;

	ifstream in;
	ofstream out;
public:
	Decode(void);
	~Decode(void);

	void load_first_value(void);
	void decode(char *infile, char *outfile);
	int decode_symbol(void);
	int get_bit(void);
};

