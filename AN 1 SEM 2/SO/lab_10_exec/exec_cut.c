/*[Exec command #2: cut]
i) Să se scrie un program C care să execute comanda cut -d: -f1,3 /etc/passwd, iar la sfârșitul execuției comenzii să afișeze textul: "Comanda cut a fost executata ...", dar numai în situația când într-adevăr s-a executat comanda cut, altfel să fie afișat textul: "Comanda cut nu a putut fi executata...".
Cerință: se va folosi primitiva execlp() și nu funcția system() !
ii) Să se rescrie programul de la punctul i), utilizând primitiva execl() în locul primitivei execlp(). */


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
	pid_t pid;
	int cod_term;

	/* creez un proces fiu, care va rula comanda cut */
	if(-1 == (pid=fork()) )
	{
		perror("Eroare la fork"); exit(1);
	}

	if (pid == 0)
	{
		/* in fiu apelez exec pentru a executa comanda dorita */
		execlp("cut","cut","-d:","-f1,3","/etc/passwd",NULL);
 
		perror("Eroare la exec");
		exit(10); /* folosim un numar mai mare, e.g. 10, si nu 1,2,... caci aceste valori mici ar putea fi returnate si de comanda cut ! */
	}

	/* (doar in parinte) cercetez cum s-a terminat procesul fiu */
	wait(&cod_term);
	if(cod_term >> 8 == 10)
		printf("Comanda cut nu a putut fi executata... (apelul exec a esuat).\n");
	else
	{
		printf("Comanda cut a fost executata...");
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
	Observație: practic, acesta este un program identic cu cel de la [Exec command #1: ls] de mai sus,
	singura diferență fiind linia de cod:
		execlp("ls","ls","-i","-l",dirname,NULL);
	care trebuie înlocuită cu:
		execlp("cut","cut","-d:","-f1,3","/etc/passwd",NULL);
	plus încă câteva modificări minore...

  ii) Rezolvare: se procedeaza similar ca la ex. [Exec command #1: ls]: în programul de mai sus, trebuie doar să se înlocuiască linia de cod:
		execlp("cut","cut","-d:","-f1,3","/etc/passwd",NULL);
  cu următoarea linie de cod:
		execl("/usr/bin/cut","cut","-d:","-f1,3","/etc/passwd",NULL);
*/

