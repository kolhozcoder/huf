#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "string.h"
#include "treenode.h"

void decode(char *archive_filename, char *destination_filename)
{
	unsigned int filesize, i, j,outbufsize,bufpos,bitpos;
	unsigned char mask[32], buf[1024], outbuf[1024] = { 0 };
	unsigned char cursym, curlen,rol;
	
	treeNode *root = NULL, *curnode;

	FILE* arch = fopen(archive_filename, "rb");
	if (arch == NULL)
	{
		printf("Cannot open file: %s\n", archive_filename);
		exit(1);
	}
	FILE* dest = fopen(destination_filename, "wb");

	fread(&filesize, 4, 1, arch);
	printf("filesize=%u\n", filesize);

	fread(mask, 1, 32, arch);
	for ( i = 0; i < 32; i++)
	{
		printf("%x\n", mask[i] & 0xff);
	}
	for (i = 0; i < 256; i++)
	{
		if (mask[i / 8] & (1 << (7 - i % 8)))
		{
			
			cursym = (unsigned char)i;
			if (root == NULL)
				root = createNode(cursym);
			curnode = root;
			fread(&curlen, 1, 1, arch);
			printf("CURLEN %u\n", curlen);

			fread(buf, 1, curlen%8==0?curlen/8:(curlen / 8 + 1), arch);

			for (j = 0; j < curlen; j++)
			{
				if(curnode->link[(buf[j / 8] & (1 << (7 - j % 8)))>>(7-j%8)]==NULL)
					curnode->link[(buf[j / 8] & (1 << (7 - j % 8))) >> (7 - j % 8)] = createNode(cursym);
				curnode = curnode->link[(buf[j / 8] & (1 << (7 - j % 8))) >> (7 - j % 8)];
			}
		}
	}
	

	memset(buf, 0, 1024);
	fread(buf, 1, 1024, arch);
	bufpos = 0;
	bitpos = 0;
	outbufsize = 0;
	for (i = 0; i < filesize; i++)
	{
		curnode = root;
		while (curnode->link[0])
		{
			curnode = curnode->link[(buf[bufpos]&(1<<(7-bitpos)))>>(7-bitpos)];
			bitpos++;
			if (bitpos == 8)
			{
				bufpos++;
				if (bufpos == 1024)
				{
					fread(buf, 1, 1024, arch);
					bufpos = 0;
				}
				bitpos = 0;
			}
		}
		outbuf[outbufsize] = curnode->value;
		outbufsize++;
		if (outbufsize == 1024)
		{
			fwrite(outbuf, 1, 1024, dest);
			outbufsize = 0;
			memset(outbuf, 0, 1024);
		}
	}
	fwrite(outbuf, 1, outbufsize, dest);

	fclose(dest);
	fclose(arch);
	return;
}