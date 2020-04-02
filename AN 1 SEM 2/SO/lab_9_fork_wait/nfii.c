/*
  Program: sablonul de creare a N fii ai procesului curent.
*/
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int N, i;
	pid_t pid;

	/* obținerea parametrului de intrare N */
	if(argc < 2)
	{
		printf("Introduceti numarul de fii: ");
		while( (1 != scanf("%d", &N)) && (N < 1) ) { fprintf(stderr,"\nEroare: nu ati introdus un numar intreg strict pozitiv! Incercati din nou..."); }
	}
	else
	{
		if(1 != sscanf(argv[1],"%d", &N)) { fprintf(stderr,"Eroare: nu ati specificat un numar intreg strict pozitiv!\n");  exit(1); }
	}

	printf("Sunt procesul initial cu PID-ul: %d, iar parintele meu este procesul cu PID-ul: %d.\n", getpid(), getppid() );

	/* bucla de producere a celor N procese fii */
	for(i = 1; i <= N; i++)
	{
		if(-1 == (pid=fork()) )
		{
			perror("Eroare la fork");  exit(2);
		}
		if(0 == pid)
		{
			printf("Sunt procesul fiu %d, avand PID-ul: %d, iar parintele meu este procesul cu PID-ul: %d.\n", i, getpid(), getppid() );
			return i;  // Important: fiul nu trebuie sa reia executia buclei for, ca sa nu creeze la randul lui alte procese fii !
		}
	}

	/* așteptarea terminării tuturor celor N fii */
	for(i = 1; i<= N; i++)  // Procesul initial va astepta terminarea celor N fii ; altfel, cateodata (i.e. nu neaparat la toate executiile) este posibil
		wait(NULL);     // ca unii fii sa apara ca fiind orfani (i.e. ca avand drept parinte procesul cu PID-ul 1)

	return 0;
}
