CC = gcc
CFLAGS = -Wall -std=c99 -g 

Worms : main.o graphics.o util.o level.o worm.o physObj.o item.o game.o anim.o team.o weapon.o
	$(CC) $(CFLAGS) -o $@ $^ -lX11 -lm -I/usr/local/include -L/usr/local/lib -lSDL

util.o : util.c util.h

graphics.o : graphics.c graphics.h util.h

level.o : level.c level.h util.h

anim.o : anim.c anim.h graphics.h

game.o : game.c game.h item.h worm.h

item.o : item.c item.h weapon.h

physObj.o : physObj.c physObj.h level.h util.h

worm.o : worm.c worm.h

team.o : team.c team.h worm.h weapon.h util.h

weapon.o : weapon.c weapon.h

main.o : main.c util.h graphics.h level.h worm.h physObj.h item.h game.h anim.h team.h weapon.h

clean:
	rm -f main.o graphics.o util.o level.o worm.o physObj.o item.o game.o anim.o team.o weapon.o Worms
