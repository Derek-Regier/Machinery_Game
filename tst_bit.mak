tst_bit.prg: tst_bit.o raster.o font.o rast_asm.o bitmaps.o
	cc68x tst_bit.o raster.o rast_asm.o font.o bitmaps.o -o tst_bit.prg
tst_bit.o: tst_bit.c raster.h
	cc68x -g -c tst_bit.c
raster.o: raster.c raster.h
	cc68x -g -c raster.c
bitmaps.o: bitmaps.c raster.h
	cc68x -g -c bitmaps.c
font.o: font.c font.h
	cc68x -g -c font.c
rast_asm.o: rast_asm.s
	gen -D -L2 rast_asm.s
clean:
	$(RM) *.o *.prg