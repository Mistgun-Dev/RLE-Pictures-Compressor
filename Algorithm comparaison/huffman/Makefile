CC=gcc
OBJ=huffman.o

huffman : 	$(OBJ)	
		$(CC) $(OBJ) -o huffman

%.o :	%.c
	$(CC) -c $<

clean:
	@rm -f *.o
	@rm -f core
	@rm -f *~
	@rm -f huffman
