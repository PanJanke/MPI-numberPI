# include <stdlib.h>
# include <stdio.h>
# include <math.h>

# include "mpi.h"

#ifndef M_PI // standardy C99 i C11 nie wymagają definiowania stałej M_PI
#define M_PI (3.14159265358979323846)
#endif




int main(int argc, char* argv[]) {

	int rank_id;
	int size;
	int i;
	int root;
	int n;	//dlugosc ciagu, liczba elementow
	int n_lok;
	int n_start, n_end;
	double suma = 0.0;
	double sum = 0.0;

	//szkielet
	MPI_Init(&argc, &argv); //inicjalizacja MPI, start wielowątkowości
	MPI_Comm_rank(MPI_COMM_WORLD, &rank_id); //pobieranie identyfikatora procesu
	MPI_Comm_size(MPI_COMM_WORLD, &size); //pobieranie ilosci dost procesow


	root = 0; //proces 0 - pobiera inf o liczbie sumowanych skladnikow 

	if (root == 0) {
		n = 100000;
	}

	MPI_Bcast(&n, 1, MPI_INT, root, MPI_COMM_WORLD); //rozeslanie danych do wszystkich procesow, 1 do wszystkiego

	if ((n % size) == 0)
		n_lok = n / size;
	else
		n_lok = (n / size) + 1;

	n_start = rank_id * n_lok;
	n_end = n_lok * (rank_id + 1);

	for (i = n_start; i < n_end; i += 2) {
		sum += (1.0) / ((2 * i) + 1);
	}
	for (i = n_start + 1; i < n_end; i += 2) {
		sum -= (1.0) / ((2 * i) + 1);
	}



	MPI_Reduce(&sum, &suma, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);

	if (rank_id == 0) {
		suma = suma * 4.0;
		printf("suma: %lf\n", suma);
		printf("systemowe pi: %lf\n", (M_PI));
	}

	MPI_Finalize();

	

	return;
}