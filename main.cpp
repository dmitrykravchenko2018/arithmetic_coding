#include "encode.h"
#include "decode.h"

int main ( int argc, char **argv)
{
	if (argc < 4)
	{
		std::cout<<"Using: name_programm <input_of_file> <output_file>"<<endl;
		return -1;
	}
	if (argv[1][0] == 'e')
	{
		Encode obj;
		obj.encode(argv[2], argv[3]);
	}
	else if (argv[1][0] == 'd')
	{
		Decode obj;
		obj.decode(argv[2], argv[3]);
	}
	return 0;
}
