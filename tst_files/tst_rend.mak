tst_rend.prg: tst_rend.o render.o model.o player.o enemy.o boss.o item.o health.o raster.o rast_asm.o bitmaps.o
	cc68x tst_rend.o render.o model.o player.o enemy.o boss.o item.o health.o raster.o rast_asm.o bitmaps.o -o tst_rend.prg
tst_rend.o: tst_rend.c render.h model.h
	cc68x -g -c tst_rend.c
render.o: render.c render.h raster.h bitmaps.h
	cc68x -g -c render.c
model.o: model.c model.h
	cc68x -g -c model.c
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
raster.o: raster.c raster.h
	cc68x -g -c raster.c
rast_asm.o: rast_asm.s
	gen -D -L2 rast_asm.s
bitmaps.o: bitmaps.c bitmaps.h
	cc68x -g -c bitmaps.c
clean:
	rm *.o tst_rend.prg