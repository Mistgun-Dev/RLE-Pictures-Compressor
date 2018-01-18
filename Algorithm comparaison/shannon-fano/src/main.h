#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define MAPSIZE 256
#define STAT

#ifdef STAT
#include <time.h>
#endif
typedef struct ptab
{
	int ch;
	float p;
} ptab;

typedef struct buffer
{
	char v;
	int size;
} buffer;


typedef struct tree
{
  int data;
  struct tree *left;
  struct tree *right;
} tree;



// pack.c
void pack(const char *input, const char *output);
void encode(int li, int ri);
void charcat(char s[], char t);
int ptablebuild(FILE *infile, ptab v[]);

// unpack.c
void unpack(const char *input, const char *output);
void buildtree(FILE *infile, tree *root, buffer *buff);

// sort.c
void swap(ptab v[], int i, int j);
void quicksort(ptab v[],int first, int last);

// bintree.c
void clear(tree *root);
int gototree(char c, tree **t);
tree* addelement(int v);

// bitop.c
void writebit(FILE *outfile, buffer *buff, char bit);
int readbit(FILE *infile, buffer *buff);

void chartobit(int val, char v[]);
int bittochar(char v[]);
