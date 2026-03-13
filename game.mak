crashers.prg: crashers.o render.o raster.o model.o synch.o asynch.o cond.o input.o bitmaps.o player.o enemy.o boss.o item.o health.o rast_asm.o
	cc68x -g -o crashers.prg crashers.o render.o raster.o model.o synch.o asynch.o cond.o input.o bitmaps.o player.o enemy.o boss.o item.o health.o rast_asm.o

crashers.o: crashers.c crashers.h
	cc68x -g -c crashers.c

render.o: render.c render.h raster.h bitmaps.h
	cc68x -g -c render.c

raster.o: raster.c raster.h
	cc68x -g -c raster.c

model.o: model.c model.h
	cc68x -g -c model.c

synch.o: synch.c synch.h
	cc68x -g -c synch.c

asynch.o: asynch.c asynch.h
	cc68x -g -c asynch.c

cond.o: cond.c cond.h
	cc68x -g -c cond.c

input.o: input.c input.h
	cc68x -g -c input.c

bitmaps.o: bitmaps.c bitmaps.h
	cc68x -g -c bitmaps.c

player.o: player.c player.h
	cc68x -g -c player.c

enemy.o: enemy.c enemy.h
	cc68x -g -c enemy.c

boss.o: boss.c boss.h
	cc68x -g -c boss.c

item.o: item.c item.h
	cc68x -g -c item.c

health.o: health.c health.h
	cc68x -g -c health.c

rast_asm.o: rast_asm.s
	gen -D -L2 rast_asm.s

clean:
	rm *.o crashers.prg