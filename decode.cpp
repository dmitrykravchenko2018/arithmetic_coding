#include "decode.h"


Decode::Decode(void)
{
	buffer = 0;
	bits_in_buf = 0;
	end_decoding = false;

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
		if ((sym_index == EOF_SYMBOL) || end_decoding)
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
	int range;
	int cum; 
	int symbol_index;

	range = high - low;
	cum = ((((value - low) + 1) * cum_freq[0] - 1) / range); // +- Нужно для то ли округления то ли преобразования, но иначе не работает
	for (symbol_index = 1; cum_freq[symbol_index] > cum; symbol_index++);
	high = low + (range * cum_freq[symbol_index - 1]) / cum_freq[0];
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
		high = 2 * high;
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
			end_decoding = true;
			return -1;
		}
		bits_in_buf= 8;
	}
	t = buffer & 1;
	buffer >>= 1;
	bits_in_buf--;
	return t;
}
