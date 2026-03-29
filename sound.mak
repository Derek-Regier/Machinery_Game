sound.prg: psg.o tst_psg.o
	cc68x -g -o sound.prg psg.o tst_psg.o

psg.o: psg.c psg.h
	cc68x -g -c psg.c

tst_psg.o: tst_psg.c psg.h
	cc68x -g -c tst_psg.c


clean:
	rm *.o