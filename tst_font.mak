tst_font.prg: tst_font.o raster.o rast_asm.o font.o
	cc68x tst_font.o raster.o rast_asm.o font.o -o tst_font.prg
tst_font.o: tst_font.c raster.h font.h
	cc68x -g -c tst_font.c
raster.o: raster.c raster.h
	cc68x -g -c raster.c
font.o: font.c font.h
	cc68x -g -c font.c
rast_asm.o: rast_asm.s
	gen -D -L2 rast_asm.s
clean:
	$(RM) *.o *.prg