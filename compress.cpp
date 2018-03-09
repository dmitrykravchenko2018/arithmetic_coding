#include "compress.h"


Compress::Compress(void)
{
	int i;
	for ( i = 0; i < NO_OF_CHARS; i++)
	{
		char_to_index[i] = i + 1;
		index_to_char[i + 1] = i;
	}
	for ( i = 0; i <= NO_OF_SYMBOLS; i++)
	{
		freq[i] = 1;
		cum_freq[i] = NO_OF_SYMBOLS - i;
	}
	freq[0] = 0;
}

Compress::~Compress(void)
{
}

void Compress::update_tables(int sym_index)
{
  int i;
  if (cum_freq[0] == MAX_FREQ)
  {
    int cum = 0;
    for ( i = NO_OF_SYMBOLS; i >= 0; i--)
    {
      freq[i] = (freq[i] + 1) / 2;
      cum_freq[i] = cum;
      cum += freq[i];
    }
  }
  for ( i = sym_index; freq[i] == freq[i - 1]; i--);
  if (i < sym_index)
  {
	int ch_i, ch_symbol;
    ch_i = index_to_char[i];
    ch_symbol = index_to_char[sym_index];
    index_to_char[i] = ch_symbol;
    index_to_char[sym_index] = ch_i;
    char_to_index[ch_i] = sym_index;
    char_to_index[ch_symbol] = i;
  }
  freq[i]++;
  while (i > 0)
  {
	i--;
    cum_freq[i]++;
  }
}
