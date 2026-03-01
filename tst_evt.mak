tst_evt: player.o enemy.o boss.o item.o health.o asynch.o synch.o cond.o model.o tst_evt.o
    cc68x -g player.o enemy.o boss.o item.o health.o asynch.o synch.o cond.o model.o tst_evt.o -o tst_evt.prg 

tst_evt.o: tst_evt.c
    cc68x -g -c tst_evt.c

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

asynch.o: asynch.c asynch.h
    cc68x -g -c asynch.c

synch.o: synch.c synch.h
    cc68x -g -c synch.c

cond.o: cond.c cond.h
    cc68x -g -c cond.c

model.o: model.c model.h
    cc68x -g -c model.c