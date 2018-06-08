OPT=-g -Wall -Wextra -Werror=return-type

EXECS=treurecom parser-01 parser-02 parser horari-01 horari-02 horari horari_m

#=====================
# Execucions de prova
#=====================

testhorari.pdf : testhorari.txt
	iconv -c -f utf8 -t ISO-8859-1 testhorari.txt | a2ps -M A4dj -1 --no-header --borders=no -o- - | ps2pdf - testhorari.pdf

testhorari.txt : treurecom parser horari
	./treurecom < assigs_gmat.txt | ./parser 2017 > linies1.txt
	./treurecom < assigs_gea.txt | ./parser 2017 > linies2.txt
	cat linies1.txt linies2.txt | ./horari 2017 16/4 > testhorari.txt

liniestesthorari : treurecom parser horari
	./treurecom < assigs_gmat.txt | ./parser 2017 ; ./treurecom < assigs_gea.txt | ./parser 2017

#=============
# Executables
#=============

treurecom : treurecom.c
	gcc -o treurecom $(OPT) treurecom.c -lm

parser-01 : parser-01.c
	gcc -o parser-01 $(OPT) parser-01.c -lm

parser-02 : parser-02.c
	gcc -o parser-02 $(OPT) parser-02.c -lm

parser : parser.c dates.o
	gcc -o parser $(OPT) parser.c dates.o -lm

horari-01 : horari-01.c dates.o
	gcc -o horari-01 $(OPT) horari-01.c dates.o -lm

horari-02 : horari-02.c dates.o
	gcc -o horari-02 $(OPT) horari-02.c dates.o -lm

horari : horari.c dates.o
	gcc -o horari $(OPT) horari.c dates.o -lm

#============
# Biblioteca
#============

dates.o : dates.c
	gcc -c $(OPT) dates.c

#========
# Neteja
#========

clean :
	rm -f *.o

cleanexecs : clean
	rm -f $(EXECS)

realclean : clean cleanexecs
	rm -f testhorari.pdf testhorari.txt
