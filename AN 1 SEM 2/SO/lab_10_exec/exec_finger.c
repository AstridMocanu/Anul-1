/*[Exec command #3: finger]
i) Să se scrie un program C care să execute comanda finger -p -m username, unde username este un parametru dat la execuția programului în linia de comandă. La sfârșitul execuției trebuie să fie afișat textul: "Comanda finger a fost executata...", dar numai în situația când într-adevăr s-a executat comanda finger, altfel să fie afișat textul: "Comanda finger nu a putut fi executata...".
Cerință: se va folosi primitiva execlp() și nu funcția system() !
ii) Să se rescrie programul de la punctul i), utilizând primitiva execl() în locul primitivei execlp(). */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int cod_term;

	if(argc != 2)
	{
		fprintf(stderr,"Programul trebuie apelat cu un parametru (un nume de utilizator).\n");  exit(1);
	}

	/* creez un proces fiu, care va rula comanda cut */
	if(-1 == (pid=fork()) )
	{
		perror("Eroare la fork");  exit(2);
	}

	if (pid == 0)
	{
		/* in fiu apelez exec pentru a executa comanda dorita */
		execlp("finger","finger","-p","-m",argv[1],NULL);
 
		perror("Eroare la exec");
		exit(10); /* folosim un numar mai mare, e.g. 10, si nu 1,2,... caci aceste valori mici ar putea fi returnate si de comanda finger ! */
	}

	/* (doar in parinte) cercetez cum s-a terminat procesul fiu */
	wait(&cod_term);
	if(cod_term >> 8 == 10)
		printf("Comanda finger nu a putut fi executata... (apelul exec a esuat).\n");
	else
	{
		printf("Comanda finger a fost executata...");
		if(cod_term == 0)
			printf(" cu succes!\n");
		else
			if( WIFEXITED(cod_term) )
				printf(", dar a esuat, terminandu-se cu codul de terminare: %d.\n", WEXITSTATUS(cod_term) );
			else
				printf(", dar a fost terminata fortat de catre semnalul: %d.\n", WTERMSIG(cod_term) );
	}
	return 0;
}

/*
	Observație: practic, acesta este un program identic cu cel de la [Exec command #1: ls] (și cu cel de la [Exec command #2: cut]) de mai sus,
	singura diferență fiind linia de cod:
		execlp("ls","ls","-i","-l",dirname,NULL);
	care trebuie înlocuită cu:
		execlp("finger","finger","-p","-m",argv[1],NULL);
	plus încă câteva modificări minore...

  ii) Rezolvare: se procedeaza similar ca la ex. [Exec command #1: ls]: în programul de mai sus, trebuie doar să se înlocuiască linia de cod:
		execlp("finger","finger","-p","-m",argv[1],NULL);
  cu următoarea linie de cod:
		execl("/usr/bin/finger","finger","-p","-m",argv[1],NULL);
*/
