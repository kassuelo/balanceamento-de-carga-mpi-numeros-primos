//mpirun -np 2 --host Aspire,Aspire m
//mpirun -np 4 --hostfile my-hosts m

#include "mpi.h"
#include <stdio.h>
#include "tempo.h"

int fprimo(int);

int main(int argc, char *argv[])
{
	printf("\nDigite um numero limite: ");
	MPI_Status status;
	int myid, numprocs;
	int total = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	if (myid == 0)
	{
		int inicio = 0;
		int fim;
		int m;
		int n; //n elementos
		printf("\n");
		scanf("%d", &n);
		tempo1();
		int menor_num = 1;
		int maior_num = n;
		int parte = ceil((double)n / numprocs);
		int arg[2];
		arg[0] = parte;
		printf("\n\nParte é %d / %d = %d \n\n", n, numprocs, parte);

		for (int i = 0; i < numprocs; i++)
		{
			for (int j = 0; j < parte; j++)
			{
				if (j % 2 == 0)
				{
					//var   qtd   tipo dest etq
					if (i == 0)
					{
						MPI_Send(&menor_num, 1, MPI_INT, i, 4, MPI_COMM_WORLD);
					}
					else
					{
						arg[1] = menor_num;
						MPI_Send(&arg, 2, MPI_INT, i, 4, MPI_COMM_WORLD);
					}
					menor_num++;
				}
				else
				{
					if (i == 0)
					{
						MPI_Send(&maior_num, 1, MPI_INT, i, 4, MPI_COMM_WORLD);
					}
					else
					{
						arg[1] = maior_num;
						MPI_Send(&arg, 2, MPI_INT, i, 4, MPI_COMM_WORLD);
					}
						maior_num--;
				}
				if (maior_num < menor_num)
				{
					j = parte;
				}
			}
		}
		int x_mestre, numero_mestre;
		for (int j = 0; j < parte; j++)
		{
			MPI_Recv(&numero_mestre, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
			printf("proc %d verificando o número = %d", myid, numero_mestre);
			x_mestre = fprimo(numero_mestre);
			if (x_mestre == 1)
			{
				total += x_mestre;
				printf(" Resultado: %d é primo!", numero_mestre);
			}
			printf("\n");
			//MPI_Send(&x_mestre, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
		}
		//mestre recebe de todos os escravos em qualquer ordem
		for (int i = 1; i < numprocs; i++)
		{
			for (int j = 0; j < parte; j++)
			{
				MPI_Recv(&m, 1, MPI_INT, MPI_ANY_SOURCE, 4, MPI_COMM_WORLD, &status);
				//printf("mestre recebeu = %d  do slave %d\n", m, status.MPI_SOURCE);
				total += m;
			}
			tempo2();
			tempoFinal("mili segundos", argv[0], MSGLOG);
			printf(" --> proc %d\n", i);
		}
		printf("\n\nTOTAL de primos é %d \n\n", total);
	}

	else
	{
		int x, numero, parte_escravo;
		int arg_recebido[2];
		MPI_Recv(&arg_recebido, 2, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
		parte_escravo = arg_recebido[0];
		numero = arg_recebido[1];
		printf("proc %d verificado o número = %d", myid, numero);
		x = fprimo(numero);
		if (x == 1)
		{
			printf(" Resultado: %d é primo!", numero);
		}
		printf("\n");
		MPI_Send(&x, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
		for (int j = 1; j < parte_escravo; j++)
		{
			MPI_Recv(&arg_recebido, 2, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
			parte_escravo = arg_recebido[0];
			numero = arg_recebido[1];
			printf("proc %d verificado o número = %d", myid, numero);
			x = fprimo(numero);
			if (x == 1)
			{
				printf(" Resultado: %d é primo!", numero);
			}
			printf("\n");
			MPI_Send(&x, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();

	return 0;
}

int fprimo(int num)
{
	int div;

	//printf("\nNumeros primos: ");
	div = 0;
	for (int i = 1; i <= num; i++)
	{
		if (num % i == 0)
		{
			div++;
		}
	}

	if (div == 2)
	{
		return 1;
	}

	return 0;
}

/*

                N  PRIME_NUMBER

                1           0
               10           4
              100          25
            1,000         168
           10,000       1,229
          100,000       9,592
        1,000,000      78,498
       10,000,000     664,579
      100,000,000   5,761,455
    1,000,000,000  50,847,534

*/