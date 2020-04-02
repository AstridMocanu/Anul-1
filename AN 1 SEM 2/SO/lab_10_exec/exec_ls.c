/*[Exec command #1: ls]
i) Să se scrie un program C care să execute comanda ls -i -l dirname, unde dirname este un parametru specificat la execuția programului în linia de comandă, sau directorul curent de lucru în caz că lipsește acest parametru. La sfârșitul execuției trebuie să fie afișat textul: "Comanda ls a fost executata ...", dar numai în situația când într-adevăr s-a executat comanda ls, altfel să fie afișat textul: "Comanda ls nu a putut fi executata...".
Cerință: se va folosi primitiva execlp() și nu funcția system() !
ii) Să se rescrie programul de la punctul i), utilizând primitiva execl() în locul primitivei execlp(). 
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int cod_term;
	char* dirname;

	dirname = (argc < 2) ? "." : argv[1];

	/* creez un proces fiu, care va rula comanda ls */
	if(-1 == (pid=fork()) )
	{
		perror("Eroare la fork"); exit(1);
	}

	if (pid == 0)
	{
		/* in fiu apelez exec pentru a executa comanda dorita */
		execlp("ls","ls","-i","-l",dirname,NULL);
 
		perror("Eroare la exec");
		exit(10); /* folosim un numar mai mare, e.g. 10, si nu 1,2,... caci aceste valori mici ar putea fi returnate si de comanda ls ! */
	}

	/* (doar in parinte) cercetez cum s-a terminat procesul fiu */
	wait(&cod_term);
	if(cod_term >> 8 == 10)
		printf("Comanda ls nu a putut fi executata... (apelul exec a esuat).\n");
	else
	{
		printf("Comanda ls a fost executata...");
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
  ii) Rezolvare: în programul de mai sus, trebuie doar să se înlocuiască linia de cod:
		execlp("ls","ls","-i","-l",dirname,NULL);
  cu următoarea linie de cod:
		execl("/bin/ls","ls","-i","-l",dirname,NULL);

  Așadar, se observă că trebuie specificată calea absolută către comanda dorită în cazul folosirii apelului execl(),
  deoarece dacă fișierul executabil este specificat printr-un nume simplu (i.e., fără cale absolută sau relativă),
  atunci acesta nu este căutat în directoarele din variabila de mediu PATH, așa cum se întâmplă în cazul apelului exelp().
*/

