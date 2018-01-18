# Shannon-Fano coding archiver

	To compile the project:
	
	$ make

	To start the program:

	$ ./src/shannon-fano

	Shannon-Fano coding archiver
	by Minh Ngo (nlminhtl@gmail.com)

	Ussage: ./shannon-fano OPTION input output
		-p	compressing
		-u	uncompressing
	
## Example

	$ cat makefile

	main:
		gcc main.c bintree.c bitop.c pack.c sort.c unpack.c -o shannon-fano

	$ ./shannon-fano -p makefile dd.sf

	code table size: 23
	c->0000
	n->0001
	 ->001
	a->010
	.->011
	o->10000
	i->10001
	p->1001
	t->10100
	m->10101
	k->1011
	r->110000
	b->110001
	-->11001

	->11010
	s->11011
	e->111000
	u->111001
	:->111010
		->111011
	f->111100
	h->111101
	g->11111
	c->0000
	n->0001
	 ->001
	a->010
	.->011
	o->10000
	i->10001
	p->1001
	t->10100
	m->10101
	k->1011
	r->110000
	b->110001
	-->11001

	->11010
	s->11011
	e->111000
	u->111001
	:->111010
		->111011
	f->111100
	h->111101
	g->11111
	time:0.000000

	$ ./shannon-fano -u dd.sf dd

	time:0.000000	

	$ md5sum dd && md5sum makefile

	e7b192cd77a633d5e11fa3ba2359fd91  dd
	e7b192cd77a633d5e11fa3ba2359fd91  makefile

# Entropy finding

	./utilities/entropy <filename>

## Example

	$ utilities/entropy shannon-fano/makefile 
	
	Symbol			Symbol Count			Probability
	"	"			1			0.013514
	"
	"			1			0.013514
	" "			8			0.108108
	"-"			2			0.027027
	"."			6			0.081081
	":"			1			0.013514
	"a"			6			0.081081
	"b"			2			0.027027
	"c"			10			0.135135
	"e"			2			0.027027
	"f"			1			0.013514
	"g"			1			0.013514
	"h"			1			0.013514
	"i"			4			0.054054
	"k"			2			0.027027
	"m"			2			0.027027
	"n"			8			0.108108
	"o"			5			0.067568
	"p"			3			0.040541
	"r"			2			0.027027
	"s"			2			0.027027
	"t"			3			0.040541
	"u"			1			0.013514
	Entropy: 4.110018
