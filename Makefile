all: FS_assg

FS_assg: assg_fs.o vector.o spheres.o color.o
	gcc -Wall -Werror -std=c99 -o FS_assg assg_fs.o vector.o spheres.o color.o -lm

assg_fs.o: src/assg.c src/vector.h src/spheres.h src/color.h
	gcc -Wall -Werror -std=c99 -DFS -c src/assg.c -o assg_fs.o

vector.o: src/vector.c src/vector.h
	gcc -Wall -Werror -std=c99 -c src/vector.c

spheres.o: src/spheres.c src/spheres.h src/vector.h
	gcc -Wall -Werror -std=c99 -c src/spheres.c

color.o: src/color.c src/color.h src/vector.h
	gcc -Wall -Werror -std=c99 -c src/color.c

clean:
	rm -f *.o FS_assg
