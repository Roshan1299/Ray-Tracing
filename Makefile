all: FS_assg

FS_assg: assg_fs.o vector.o spheres.o color.o
	gcc -Wall -Werror -std=c99 -o FS_assg assg_fs.o vector.o spheres.o color.o -lm

assg_fs.o: src/main.c src/vector.h src/spheres.h src/color.h
	gcc -Wall -Werror -std=c99 -DFS -c src/main.c -o assg_fs.o

vector.o: src/vector.c src/vector.h
	gcc -Wall -Werror -std=c99 -c src/vector.c

spheres.o: src/spheres.c src/spheres.h src/vector.h
	gcc -Wall -Werror -std=c99 -c src/spheres.c

color.o: src/color.c src/color.h src/vector.h
	gcc -Wall -Werror -std=c99 -c src/color.c

clean:
	rm -f *.o FS_assg

# Convenience targets for testing and viewing
test: FS_assg
	./FS_assg sample_input.txt output.ppm
	python3 ppm_to_png.py output.ppm

view: output.ppm
	python3 view_image.py output.ppm

# Run with custom input file
run: FS_assg
	@if [ -z "$(INPUT)" ]; then \
		echo "Usage: make run INPUT=input_file.txt OUTPUT=output_file.ppm"; \
		echo "Example: make run INPUT=sample_input.txt OUTPUT=my_output.ppm"; \
		exit 1; \
	fi
	./FS_assg $(INPUT) $(OUTPUT)
	python3 ppm_to_png.py $(OUTPUT)

# View custom PPM file
view-custom:
	@if [ -z "$(FILE)" ]; then \
		echo "Usage: make view-custom FILE=input.ppm"; \
		exit 1; \
	fi
	python3 view_image.py $(FILE)
