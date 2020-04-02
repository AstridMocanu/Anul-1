/*
  Program: sablonul de creare a N fii ai procesului curent, varianta iterativă.
*/
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int N, i, codterm;
	pid_t pid;

	/* obținerea parametrului de intrare N */
	if(argc < 2)
	{
		printf("Introduceti numarul de procese: ");
		while( (1 != scanf("%d", &N)) && (N < 1) ) { fprintf(stderr,"\nEroare: nu ati introdus un numar intreg strict pozitiv! Incercati din nou..."); }
	}
	else
	{
		if(1 != sscanf(argv[1],"%d", &N)) { fprintf(stderr,"Eroare: nu ati specificat un numar intreg strict pozitiv!\n");  exit(1); }
	}

	/* bucla de producere a celor N procese "înlănțuite" */
	for(i = 1; i < N; i++)
	{
		if(-1 == (pid=fork()) )
		{
			perror("Eroare la fork");  exit(2);
		}

		if(0 != pid)
		{
			wait(&codterm); // Aici fiecare proces va astepta terminarea procesului fiu direct, ca sa-i obtina codul de terminare
					// Notă: chiar daca nu se cerea afisarea codului de terminare, tot ar fi nevoie aici de un apel wait(NULL) ! Altfel, cateodata (i.e. nu
					// neaparat la toate executiile) este posibil ca unele procese sa apara ca fiind orfane (i.e. ca avand drept parinte procesul cu PID-ul 1)

			// printf("Sunt procesul %d din lantul de procese, avand PID-ul: %d, parintele meu este procesul cu PID-ul: %d, iar fiul creat de mine "
			//       "are PID-ul: %d si s-a terminat cu codul de terminare: %d.\n", i, getpid(), getppid(), pid, codterm >> 8 );

			/* afisare prescurtata: */
			printf("Sunt procesul %d, avand PID-ul: %d, parintele are PID-ul: %d, iar fiul creat are PID-ul: %d si s-a terminat cu codul: %d.\n",
						i, getpid(), getppid(), pid, codterm>>8 );

			return i;  // Important: tatal nu trebuie sa reia executia buclei for, ca sa nu creeze un al doilea proces fiu direct, apoi un al treilea, s.a.m.d.
		}
		else
		{
			if(i == N-1)  // ultimul proces din lant, care nu va crea un proces fiu
			{
				printf("Sunt procesul ultim, %d din lantul de procese, avand PID-ul: %d, iar parintele meu este procesul cu PID-ul: %d.\n", N, getpid(), getppid() );
				return N;
			}
		}

	}//end for

	return 0;
}
