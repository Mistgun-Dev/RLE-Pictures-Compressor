#include "main.h"

void
showhelp()
{
	printf("Shannon-Fano coding archiver\nby Minh Ngo (nlminhtl@gmail.com)\n\n");
	printf("Ussage: ./shannon-fano OPTION input output\n\t-p\tcompressing\n\t-u\tuncompressing\n");
}

int
main(int argc, char *argv[])
{
	if (argc != 4 || argv[1][0] != '-' || strlen(argv[1]) != 2)
		{
			showhelp();
			return -1;
		}

	switch (argv[1][1])
		{
			case 'p':
				pack(argv[2], argv[3]);
				break;
			case 'u':
				unpack(argv[2], argv[3]);
				break;
			default:
				showhelp();
				return -1;
		}

	return 0;
}
