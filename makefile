SRC = ray.c vector.c sphere.c plane.c light.c box.c

OBJ = ray.o vector.o sphere.o plane.o light.o box.o

PROG = project3

$(PROG) : $(OBJ) 
	gcc $(SRC) -o $(PROG) -lm

$(OBJ) : $(SRC)   

clean:
	rm *.o

cleaner:
	rm *~

