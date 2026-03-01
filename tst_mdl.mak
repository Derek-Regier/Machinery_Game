tst_mdl.prg: tst_mdl.o model.o player.o enemy.o boss.o item.o health.o cond.o
	cc68x tst_mdl.o model.o player.o enemy.o boss.o item.o health.o cond.o -o tst_mdl.prg
tst_mdl.o: tst_mdl.c model.h
	cc68x -g -c tst_mdl.c
model.o: model.c model.h
	cc68x -g -c model.c
player.o: player.c player.h
	cc68x -g -c player.c
enemy.o: enemy.c
	cc68x -g -c enemy.c
boss.o: boss.c
	cc68x -g -c boss.c
item.o: item.c item.h
	cc68x -g -c item.c
health.o: health.c health.h
	cc68x -g -c health.c
cond.o: cond.c cond.h
	cc68x -g -c cond.c
clean:
	rm *.o tst_mdl.prg