/*[Pipe commands #1]
Să se scrie un program C care "simulează" comanda înlănțuită:
UNIX> grep /bin/bash /etc/passwd | cut -d: -f1,3-5
Cerință: pentru executarea comenzilor simple din lanț se vor utiliza apeluri de sistem de tip exec() și nu apelul system(). */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
	pid_t pid;
	int p[2];

	if(-1 == pipe(p) )
	{
		fprintf(stderr, "Eroare: nu pot crea un canal anonim, errno=%d\n",errno);  perror("Cauza erorii");  exit(1);
	}

	if(-1 == (pid=fork()) )
	{
		perror("Eroare: nu am putut crea un fiu. Cauza erorii");  exit(2);
	}

	if(pid == 0)
	{
		if(-1 == dup2(p[1],1) )
		{
			perror("Eroare: redirectarea iesirii standard in fiu a esuat. Cauza erorii");  exit(3);
		}

		execlp("grep","grep","/bin/bash","/etc/passwd",NULL);
		perror("Eroare la execlp() in fiu. Cauza erorii");  exit(4);
	}
	else
	{
		close(p[1]); // necesar pentru ca sa se termine executia comenzii cut (prin citirea EOF din canal)

		if(-1 == dup2(p[0],0) )
		{
			perror("Eroare: redirectarea intrarii standard in parinte a esuat. Cauza erorii");  exit(5);
		}

		execlp("cut","cut","-d:","-f1,3-5",NULL);
		perror("Eroare la execlp() in parinte. Cauza erorii");  exit(6);
	}

	return 0;  // fluxul de execuție nu va ajunge niciodată aici!
}

/*
  Observație (valabilă atât pentru această problemă, cât și pentru problema următoare):

	După lansarea programului în execuție, interpretorul bash va reafișa prompterul imediat după terminarea execuției
	procesului inițial (cel aflat în foreground), chiar dacă execuțiile proceselor fii posibil să nu se fi încheiat.
	Ca urmare, pentru a garanta afișarea prompterului întotdeauna la sfârșitul execuției întregului program, este
	suficient ca procesul inițial (cel care creează procesele fii necesare pentru comenzile din lanț) să fie responsabil
	cu execuția (prin execlp) a ultimei comenzi din lanțul de comenzi specificat în enunțul problemei.
*/

