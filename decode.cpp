#include "decode.h"
#include <stdexcept>


Decode::Decode(void)
{
	buffer = 0;
	bits_in_buf = 0;
	eof_reads = 0;

	low = 0;
	high = MAX_VALUE;
}

Decode::~Decode(void)
{
}

void Decode::load_first_value(void)
{
	value = 0;
	for (int i = 1; i <= CODE_VALUE; i++)
		value = 2 * value + get_bit();
}

void Decode::decode(char *infile, char *outfile)
{
	in.open(infile, ios::binary | ios::in);
	out.open(outfile, ios::binary | ios::out);
	if (!in || !out)
	{
		cout<<"Error: Can`t open file"<<endl;
		return;
	}
	load_first_value();
	while (true)
	{
		int ch;
		int sym_index;
		sym_index = decode_symbol();
		if (sym_index == EOF_SYMBOL)
			break;
		ch = index_to_char[sym_index];
		out.put(ch);
		update_tables(sym_index);
	}
	cout<<"Decoding is done"<<endl;
	in.close();
	out.close();
}

int Decode::decode_symbol(void)
{
	long range;
	int cum; 
	int symbol_index;

	range = high - low + 1;
	cum = ((((value - low) + 1) * cum_freq[0] - 1) / range);
	for (symbol_index = 1; cum_freq[symbol_index] > cum; symbol_index++);
	high = low + (range * cum_freq[symbol_index - 1]) / cum_freq[0] - 1;
	low = low + (range * cum_freq[symbol_index]) / cum_freq[0];
	for (;;)
	{
		if (high < HALF)
		{
		}
		else if (low >= HALF)
		{
			value -= HALF;
			low -= HALF;
			high -= HALF;
		}
		else if (low >= FIRST_QTR && high < THIRD_QTR)
		{
			value -= FIRST_QTR;
			low -= FIRST_QTR;
			high -= FIRST_QTR;
		}
		else
			break;
		low = 2 * low;
		high = 2 * high + 1;
		value = 2 * value + get_bit();
	}
	return symbol_index;
}

int Decode::get_bit(void)
{
	int t;
	if (bits_in_buf == 0)
	{
		buffer = in.get();
		if (buffer == EOF)
		{
			/* decoder may need padding past physical EOF to recognize the encoded EOF symbol, 
			   limit eight-bit buffer refills to prevent unlimited padding on invalid input;
			   14 refills for CODE_VALUE = 16 because finalization explicilty emits 2 oposite termination bits */
			if (++eof_reads > CODE_VALUE - 2)
				throw std::runtime_error("Bad input file");
			/* supply eight arbitrary padding bits using a positive value so subsequent shifts
			   do not depend on how the negative EOF sentinel is right-shifted. */
			buffer = 0xff;
		}
		bits_in_buf= 8;
	}
	t = buffer & 1;
	buffer >>= 1;
	bits_in_buf--;
	return t;
}
