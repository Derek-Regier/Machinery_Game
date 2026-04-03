tst_isr.prg: tst_isr.o isr.o input.o raster.o rast_asm.o font.o isr_asm.o
	cc68x tst_isr.o isr.o input.o raster.o rast_asm.o font.o isr_asm.o -o tst_isr.prg
tst_isr.o: tst_isr.c isr.h input.h raster.h font.h TYPES.H
	cc68x -g -c tst_isr.c
isr.o: isr.c isr.h TYPES.H
	cc68x -g -c isr.c
input.o: input.c input.h TYPES.H
	cc68x -g -c input.c
raster.o: raster.c raster.h TYPES.H
	cc68x -g -c raster.c
rast_asm.o: RAST_ASM.S
	gen -D -L2 RAST_ASM.S
font.o: font.c font.h TYPES.H
	cc68x -g -c font.c
isr_asm.o: ISR_ASM.S
	gen -D -L2 ISR_ASM.S
clean:
	rm *.o tst_isr.prg