#include "main.h"

void
quicksort(ptab v[],int first, int last)
{
		int i = first, j = last;
		ptab x = v[(first + last) / 2];

		do {
				while (v[i].p > x.p) i++;
				while (v[j].p < x.p) j--;

				if(i <= j)
					{
						if (i < j)
							swap(v, i, j);
						i++;
						j--;
					}
		} while (i <= j);

		if (i < last)
			quicksort(v, i, last);
		if (first < j)
				quicksort(v, first,j);
}

void
swap(ptab v[], int i, int j)
{
	ptab temp;
	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}
