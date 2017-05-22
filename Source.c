#define _CRT_SECURE_NO_WARNINGS
#define chen
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "encode.h"
#include "decode.h"

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		printf("Use the following format pls:\n./huf e encoding_filename arhive_filename\nor\n./huf d arhive_filename destination_filename\n");
		exit(1);
	}
	if (strcmp(argv[1], "e") == 0)
	{
		encode(argv[2], argv[3]);
	}
	else if (strcmp(argv[1], "d") == 0)
	{
		decode(argv[2], argv[3]);
	}
	else
	{
		printf("Use the following format pls:\n./huf e encoding_filename arhive_filename\nor\n./huf d arhive_filename destination_filename\n");
		exit(1);
	}
	return 0;
}