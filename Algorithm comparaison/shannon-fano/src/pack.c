#include "main.h"

static ptab ptable[MAPSIZE];
static char codes[MAPSIZE][256];

void
pack(const char *input, const char *output)
{
#ifdef STAT
	clock_t time1, time2;
	time1 = clock();
#endif
	int c, i, j;

	FILE *infile = fopen(input, "r");
	assert(infile);

	int size = ptablebuild(infile, ptable);

	encode(0, size - 1);

	printf("code table size: %d\n", size);

#ifdef STAT
	FILE *codetable = fopen("codetable", "wb");
	assert(codetable);
	for (i = 0; i < size; ++i)
		{
			fprintf(codetable, "%c %s %f \n", ptable[i].ch, codes[ptable[i].ch], ptable[i].p);
			printf("%c->%s\n", ptable[i].ch, codes[ptable[i].ch]);
		}
	fclose(codetable);
#endif
	
	for (i = 0; i < size; ++i)
		printf("%c->%s\n", ptable[i].ch, codes[ptable[i].ch]);
	
	FILE *outfile = fopen(output, "wb");
	assert(outfile);
	
	putc(size - 1, outfile);

	buffer buff;
	buff.size = buff.v = 0;

	char codesize[8], codebit[8], *ch;
	for (i = 0; i < size; ++i)
		{
			c = ptable[i].ch;
			chartobit(c, codebit);
			for (j = 0; j < 8; ++j)
				writebit(outfile, &buff, codebit[j]); // 8 bits of the code

			chartobit(strlen(codes[c]) - 1, codesize);
			for (j = 0; j < 8; ++j)
				writebit(outfile, &buff, codesize[j]); // size of code
			
			j = -1;
			ch = codes[c];
			while (ch[++j] != '\0')
				writebit(outfile, &buff, ch[j]); // code
		}

	fseek(infile, 0, SEEK_SET);
	
	while ((c = getc(infile)) != EOF)
		{
			ch = codes[c];
			j = -1;
			while (ch[++j] != '\0')
				writebit(outfile, &buff, ch[j]);
		}
	if (buff.size != 8)
		putc(buff.v, outfile);
	
	putc(buff.size, outfile);

	fclose(outfile);
	fclose(infile);

#ifdef STAT
	time2 = clock();
	printf("time:%f\n", (double)(time2 - time1) / (double)CLOCKS_PER_SEC);
#endif
}

int
ptablebuild(FILE *infile, ptab ptable[])
{
	int freq_table[MAPSIZE], i, c;

	unsigned long total = 0;
	for (i = 0; i < MAPSIZE; ++i)
			freq_table[i] = 0;

	while ((c = getc(infile)) != EOF)
		{
			freq_table[c]++;
			total++;
		}

	double ftot = (double)total;

	int size = 0;
	for (i = 0; i < MAPSIZE; ++i)
		{
			if (!freq_table[i])
				continue;
			ptable[size].ch = i;
			ptable[size].p = (double)freq_table[i] / ftot;
			size++;
		}
	
	quicksort(ptable, 0, size);
	return size;
}

void
encode(int li, int ri)
{
	if (li == ri)
		return;

	int i, isp;
	float p, phalf;

	if (ri - li == 1)
		{
			charcat(codes[ptable[li].ch], '0');
			charcat(codes[ptable[ri].ch], '1');
		}
	else
		{
			phalf = 0;
			for(i = li; i <= ri; ++i)
				phalf += ptable[i].p;

			p = 0;
			isp = -1;
			phalf *= 0.5f;
			for(i = li; i <= ri; ++i)
				{
					if(p <= phalf)
						charcat(codes[ptable[i].ch], '0');
					else
						{
							charcat(codes[ptable[i].ch], '1');
							if(isp < 0)
								isp = i;
						}
					p += ptable[i].p;
				}

			if (isp < 0)
				isp = li + 1;

			encode(li, isp - 1);
			encode(isp, ri);

		}

}

void
charcat(char s[], char t)
{
	int i = 0;
	while (s[i] != '\0')
		i++;
	s[i++] = t;
	s[i++] = '\0';
}
