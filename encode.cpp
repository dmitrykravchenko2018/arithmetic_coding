#include "encode.h"


Encode::Encode(void)
{
	buffer = 0;
	bits_in_buf = 0;

	low = 0;
	high = MAX_VALUE;
	opposite_bits = 0;
}

Encode::~Encode(void)
{
}

void Encode::encode(char *infile, char *outfile)
{
	in.open(infile, ios::binary | ios::in);
	out.open(outfile, ios::binary | ios::out);
	if (!in || !out)
	{
		cout<<"Error: Can`t open file"<<endl;
		return;
	}
	while (true)
	{
		int ch;
		int symbol;
		ch = in.get();
		if (in.eof())
		{
			cout<<"Encoding is done"<<endl;
			break;
		}
		symbol = char_to_index[ch];
		encode_symbol(symbol);
		update_tables(symbol);
	}
	encode_symbol(EOF_SYMBOL);
	end_encoding();
	in.close();
	out.close();
}

void Encode::encode_symbol(int symbol)
{
	int range;

	range = high - low;
	high = low + (range * cum_freq [symbol - 1]) / cum_freq [0];
	low = low + (range * cum_freq [symbol]) / cum_freq [0];
	for (;;)
	{
	if (high < HALF)
		output_bits(0);
	else if (low >= HALF)
	{
		output_bits(1);
		low -= HALF;
		high -= HALF;
	}
	else if (low >= FIRST_QTR && high < THIRD_QTR)
	{
		opposite_bits++;
		low -= FIRST_QTR;
		high -= FIRST_QTR;
	}
	else
		break;
	low = 2 * low;
	high = 2 * high;
	}
}

void Encode::end_encoding(void)
{
  opposite_bits++;
  if (low < FIRST_QTR)
    output_bits(0);
  else
    output_bits(1);

  out.put(buffer >> bits_in_buf);
}

void Encode::output_bits(int bit)
{
	write_bit(bit);
	while (opposite_bits > 0)
	{
		write_bit(!bit);
		opposite_bits--;
	}
}

void Encode::write_bit(int bit)
{
	buffer >>= 1;
	if (bit)
	buffer |= 0x80;
	bits_in_buf++;
	if (bits_in_buf == 8)
	{
		out.put(buffer);
		bits_in_buf = 0;
	}
}
