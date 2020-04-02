#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

int N;

void creaza_fiu(int i);	 // Fiecare apel al acestei functii va crea un singur proces, i.e. va executa un singur apel fork() !

int main(int argc, char* argv[])
{
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

	/* apelul recursiv ce creeaza lista de N procese */
	creaza_fiu(1);

	return 0;
}

void creaza_fiu(int i)
{
	int codterm;
	pid_t pid;

	if(i < N)
	{
		if(-1 == (pid=fork()) )
		{
			perror("Eroare la fork");  exit(2);
		}

		if(0 != pid)
		{
			wait(&codterm); // Aici fiecare proces va astepta terminarea prcesului fiu direct, ca sa-i obtina codul de terminare
					// Notă: chiar daca nu se cerea afisarea codului de terminare, tot ar fi nevoie aici de un wait(NULL) ! Altfel, cateodata (i.e. nu
					// neaparat la toate executiile) este posibil ca unele procese sa apara ca fiind orfane (i.e. ca avand drept parinte procesul cu PID-ul 1)

				//printf("Sunt procesul %d din lantul de procese, avand PID-ul: %d, parintele meu este procesul cu PID-ul: %d, iar fiul creat de mine "
				//       "are PID-ul: %d si s-a terminat cu codul de terminare: %d.\n", i, getpid(), getppid(), pid, codterm>>8 );

			/* afisare prescurtata: */
			printf("Sunt procesul %d, avand PID-ul: %d, parintele are PID-ul: %d, iar fiul creat are PID-ul: %d si s-a terminat cu codul: %d.\n",
					i, getpid(), getppid(), pid, codterm>>8 );

			exit(i); // Important: tatal nu trebuie sa reia executia buclei for, ca sa nu creeze un al doilea proces fiu direct, apoi un al treilea, s.a.m.d.
		}
		else
		{
			creaza_fiu(i+1);	// apelul recursiv
		}
	}
	else  // adica i==N, deci e vorba de ultimul proces din lant, care nu va crea un proces fiu
	{
		printf("Sunt procesul ultim, %d din lantul de procese, avand PID-ul: %d, iar parintele meu este procesul cu PID-ul: %d.\n",
				N, getpid(), getppid() );
		exit(N);
	}
}

