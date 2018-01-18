#include "main.h"

void
unpack(const char *input, const char *output)
{
#ifdef STAT
	clock_t time1, time2;
	time1 = clock();
#endif
	FILE *infile = fopen(input, "r");
	assert(infile);

	fseek(infile, -1L, SEEK_END);
	long last = ftell(infile);
	int buffsize = getc(infile);
	fseek(infile, 0, SEEK_SET);

	int i, ch, c;

	tree *root = addelement(EOF);

	buffer buff;
	buff.v = buff.size = 0;

	buildtree(infile, root, &buff);

	FILE *outfile = fopen(output, "w");
	assert(outfile);
	
	tree *temp = root;
	while ((c = readbit(infile, &buff)) != EOF)
		{
			if (ftell(infile) == last)
				break;
			else if ((ch = gototree(c, &temp)) != EOF)
				{
					putc(ch, outfile);
					temp = root;
				}
		}

		if ((ch = gototree(c, &temp)) != EOF)
			{
				putc(ch, outfile);
				temp = root;
			}

		for (i = 0; i < buffsize - 1; ++i)
			{
				c = readbit(infile, &buff);
				if ((ch = gototree(c, &temp)) != EOF)
					{
						putc(ch, outfile);
						temp = root;
					}
			}
	clear(root);	
	fclose(outfile);
	fclose(infile);

#ifdef STAT
	time2 = clock();
	printf("time:%f\n", (double)(time2 - time1) / (double)CLOCKS_PER_SEC);
#endif
}

void
buildtree(FILE *infile, tree *root, buffer *buff)
{
	int table_size = getc(infile);

	char codebit[8], codesize[8];
	int i, j, c, ch, code_size;

	tree *temp;

	for (i = 0; i <= table_size; ++i)
		{
			for (j = 0; j < 8; ++j)
				codebit[j] = readbit(infile, buff);
			ch = bittochar(codebit);

			for (j = 0; j < 8; ++j)
				codesize[j] = readbit(infile, buff);
			code_size = bittochar(codesize);

			temp = root;
			for (j = 0; j <= code_size; ++j)
				{
					c = readbit(infile, buff);
					if (c == '0')
						{
							if (temp->left == NULL)
								temp->left = addelement(EOF); 
							temp = temp->left;
						}
					else
						{
							if (temp->right == NULL)
								temp->right = addelement(EOF);
							temp = temp->right;
						}
				}
			temp->data = ch;
		}

}
