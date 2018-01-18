CC=gcc
OBJ=lzwfilter.o lzw-lib.o

lzw : 	$(OBJ)	
		$(CC) $(OBJ) -o lzw lzw-lib.h

%.o :	%.c
	$(CC) -c $<

clean:
	@rm -f *.o
	@rm -f core
	@rm -f *~
	@rm -f lzw
