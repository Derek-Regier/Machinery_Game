tst_evt.prg: player.o enemy.o boss.o item.o health.o asynch.o synch.o cond.o model.o tst_evt.o
	cc68x -g player.o enemy.o boss.o item.o health.o asynch.o synch.o cond.o model.o tst_evt.o -o tst_evt.prg
tst_evt.o: tst_evt.c asynch.h synch.h cond.h model.h
	cc68x -g -c tst_evt.c
player.o: player.c player.h model.h
	cc68x -g -c player.c
enemy.o: enemy.c enemy.h model.h
	cc68x -g -c enemy.c
boss.o: boss.c boss.h model.h
	cc68x -g -c boss.c
item.o: item.c item.h model.h
	cc68x -g -c item.c
health.o: health.c health.h model.h
	cc68x -g -c health.c
asynch.o: asynch.c asynch.h model.h
	cc68x -g -c asynch.c
synch.o: synch.c synch.h model.h
	cc68x -g -c synch.c
cond.o: cond.c cond.h model.h
	cc68x -g -c cond.c
model.o: model.c model.h
	cc68x -g -c model.c
clean:
	rm *.o tst_evt.prg