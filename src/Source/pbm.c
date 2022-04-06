#include "../Header/pbm.h"

#include <stdlib.h>
#include <string.h>



// please, refer to DOCUMENTATION for technical details



static byte nextint(FILE* f);
static byte nextbit(FILE* f, byte* buf, size_t* buf_avail);

static byte nextint(FILE* f)
{
	char c;
	do
	{
		if (feof(f))
		{
			fprintf(stderr, "Unexpected end of file\n");
			exit(1);
		}
		c = fgetc(f);

		if (c != '#')
			continue;

		// skip comments
		do
		{
			if (feof(f))
			{
				fprintf(stderr, "Unexpected end of file in comment\n");
				exit(1);
			}
			c = fgetc(f);
		}
		while (c != '\n');

	} while (strchr(" \n\r", c));

	if (!('0' <= c && c <= '9'))
	{
		fprintf(stderr, "Unexpected character '%c'\n", c);
		exit(1);
	}

	byte res = 0;
	while ('0' <= c && c <= '9')
	{
		res *= 10;
		res += c - '0';
		if (feof(f))
			break;
		c = fgetc(f);
	}
	return res;
}

static byte nextbit(FILE* f, byte* buf, size_t* buf_avail)
{
	if (!*buf_avail)
	{
		*buf_avail = fread(buf, 1, 1, f);
	}
	return (*buf >> (--(*buf_avail))) & 1;
}

void load_pbm(scanner_t* scanner, FILE* f)
{
	fgetc(f); // reads 'P'
	char format = fgetc(f); // reads format (1-6)

	byte w = nextint(f);
	byte h = nextint(f);

	if (w != h)
	{
		fprintf(stderr, "I only handle square images\n");
		exit(1);
	}

	byte* d = (byte*) malloc(sizeof(byte) * w*h);
	if (!d)
	{
		fprintf(stderr, "Could not allocate memory\n");
		exit(1);
	}

	scanner->d = d;
	scanner->s = w;

	if (format == '1')
	{
		for (size_t i = 0; i < h; i++)
		for (size_t j = 0; j < w; j++)
			P(i, j) = nextint(f);
	}
	else if (format == '4')
	{
		byte buf = 0;
		size_t buf_avail = 0;
		for (size_t i = 0; i < h; i++)
		{
			for (size_t j = 0; j < w; j++)
				P(i, j) = nextbit(f, &buf, &buf_avail);
			buf_avail = 0;
		}
	}
	else
	{
		fprintf(stderr, "Unsupported format '%c'\n", format);
		exit(1);
	}
}

void into_pbm(scanner_t* scanner, const char* filename)
{
	FILE* fptr;
	fptr = fopen(filename,"w");

	if (fptr == NULL)
	{
		fprintf(stderr, "Error creating result file!");
		exit(1);
	}
	fprintf(fptr, "P1\n%zu %zu\n", scanner->s, scanner->s);

	
	for (size_t i = 0; i < scanner->s; i++)
	{
		for (size_t j = 0; j < scanner->s; j++)
		{
			if(scanner->d[i*scanner->s+j])
				fprintf(fptr, "1 ");
			else
				fprintf(fptr, "0 ");
		}
		fprintf(fptr, "\n");
	}
	
	fclose(fptr);
}