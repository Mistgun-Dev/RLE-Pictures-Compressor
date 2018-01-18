#include "main.h"

int
bittochar(char v[])
{
	int i, temp = 0;
	for (i = 0; i < 8; ++i)
		if (v[7 - i] == '1')
			temp += (1 << i);
	return temp;
}

void
chartobit(int val, char v[])
{
	int i;
	for (i = 7; i > 0; --i)
		{
			v[i] = (val % 2 == 1) ? '1' : '0';
			val /= 2;
		}
	v[0] = (val % 2 == 1) ? '1' : '0';
}

void
writebit(FILE *outfile, buffer *buff, char bit)
{
	if (buff->size == 8)
		{
			putc(buff->v, outfile);
			buff->v = buff->size = 0;
		}

	if (bit == '1')
		buff->v |= (1 << buff->size);
	else
		buff->v &= ~(1 << buff->size);
	++(buff->size);
}

int
readbit(FILE *infile, buffer *buff)
{
	if (buff->size == 0)
		{
			buff->v = getc(infile);
			buff->size = 8;
		}
	
	--(buff->size);
	return (((buff->v & (1 << (7 - buff->size)))
				>> (7 - buff->size)) == 1) ? '1' : '0'; 
}
