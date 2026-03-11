crashers.prg: crashers.o render.o raster.o model.o synch.o async.o cond.o input.o rast_asm.o
	cc68x -g -o crashers.prg crashers.o render.o raster.o model.o synch.o async.o cond.o input.o rast_asm.o
crashers.o: crashers.c render.h raster.h model.h synch.h asynch.h cond.h input.h types.h
	cc68x -g -c crashers.c
render.o: render.c render.h
	cc68x -g -c render.c
raster.o: raster.c raster.h
	cc68x -g -c raster.c
model.o: model.c model.h
	cc68x -g -c model.c
synch.o: synch.c synch.h
	cc68x -g -c synch.c
async.o: async.c asynch.h
	cc68x -g -c async.c
cond.o: cond.c cond.h
	cc68x -g -c cond.c
input.o: input.c input.h
	cc68x -g -c input.c
rast_asm.o: rast_asm.s
	gen -D -L2 rast_asm.s