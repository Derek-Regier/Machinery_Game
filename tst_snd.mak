tst_snd.prg: psg.o tst_snd.o music.o effects.o
	cc68x -g -o tst_snd.prg psg.o tst_snd.o music.o effects.o

psg.o: psg.c psg.h
	cc68x -g -c psg.c

music.o: music.c music.h
	cc68x -g -c music.c

effects.o: effects.c effects.h 
	cc68x -g -c effects.c 

tst_snd.o: tst_snd.c psg.h
	cc68x -g -c tst_snd.c

clean:
	rm *.o