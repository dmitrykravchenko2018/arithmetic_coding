#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;
using std::ios;

const int CODE_VALUE = 16;
const int MAX_VALUE = ((1 << CODE_VALUE) - 1);
const int MAX_FREQ = 16383;

const int FIRST_QTR = (MAX_VALUE / 4 + 1);
const int HALF = (2 * FIRST_QTR);
const int THIRD_QTR = (3 * FIRST_QTR);

const int NO_OF_CHARS = 256;
const int EOF_SYMBOL = (NO_OF_CHARS + 1);
const int NO_OF_SYMBOLS = (NO_OF_CHARS + 1);

class Compress
{
public:
	unsigned char index_to_char [NO_OF_SYMBOLS];
	int char_to_index [NO_OF_CHARS];
	int cum_freq [NO_OF_SYMBOLS + 1];
	int freq [NO_OF_SYMBOLS + 1];

	Compress(void);
	~Compress(void);
	void update_tables(int sym_index);

};

