#include "encode.h"
#include "decode.h"
#include <stdexcept>

int main ( int argc, char **argv)
{
	if (argc < 4)
	{
		std::cout<<"Usage: "<<argv[0]<<" <e|d> <input_file> <output_file>"<<endl;
		return -1;
	}
	try
	{
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
	}
	catch (const std::runtime_error &error)
	{
		std::cerr << "Error: " << error.what() << endl;
		return 1;
	}
	return 0;
}
