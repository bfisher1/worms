CC = gcc
CFLAGS = -Wall -std=c99 -g 

Worms : main.o graphics.o util.o level.o worm.o physObj.o item.o game.o anim.o team.o weapon.o ./Queue/queue.o text.o stamp.o ./List/ArrayList.o
	$(CC) $(CFLAGS) -o $@ $^ -lX11 -lm -I/usr/local/include -L/usr/local/lib -lSDL

util.o : util.c util.h

./Queue/queue.o : ./Queue/queue.c ./Queue/queue.h

./List/ArrayList.o : ./List/ArrayList.c ./List/ArrayList.h

graphics.o : graphics.c graphics.h util.h

level.o : level.c level.h util.h

anim.o : anim.c anim.h graphics.h

game.o : game.c game.h item.h worm.h weapon.o ./Queue/queue.h util.h text.h List/ArrayList.h

item.o : item.c item.h weapon.h util.h anim.h

physObj.o : physObj.c physObj.h level.h util.h

worm.o : worm.c worm.h anim.h

team.o : team.c team.h worm.h weapon.h util.h List/ArrayList.h

weapon.o : weapon.c weapon.h physObj.h level.h game.h ./Queue/queue.h item.h util.h List/ArrayList.h anim.h

stamp.o : stamp.c stamp.h anim.h level.h

text.o : text.c text.h

main.o : main.c util.h graphics.h level.h worm.h physObj.h item.h game.h anim.h team.h weapon.h ./Queue/queue.h List/ArrayList.h

clean:
	rm -f main.o graphics.o util.o level.o worm.o physObj.o item.o game.o anim.o team.o weapon.o Worms ./Queue/queue.o text.o stamp.o ./List/ArrayList.o
