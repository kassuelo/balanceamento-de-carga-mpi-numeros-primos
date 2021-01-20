
all: compile
	
compile:
	clear 
	mpicc 	-o m 	primo-mpi.c -lm

	@echo "Compilaçao concluida"

clean:
	rm -rf ?
	rm -rf ??


