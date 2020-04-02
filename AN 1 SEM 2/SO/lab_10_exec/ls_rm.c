#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[])
{
	char nume_dir[100];
	int cod_term;

	// Pasul #1: obtinerea numelui directorului de prelucrat
if(argc > 1)
	{
		strcpy(nume_dir,argv[1]);
	}
	else
	{
		printf("Introduceti numele directorului dorit:");  scanf("%s", nume_dir);
	}

	// Pasul #2: invocarea primei comenzi: ls -l nume_dir
int p;
	switch( p=fork() )
	{
		case -1:	perror("Eroare la primul fork");  exit(1);

		case  0:	execlp("ls","ls","-l",nume_dir,NULL);
perror("Eroare la primul exec");  exit(2);


default:	wait(&cod_term);
				if( WIFEXITED(cod_term) )
					printf("Comanda ls a rulat, terminandu-se cu codul de terminare: %d.\n", WEXITSTATUS(cod_term) );
				else
					printf("Comanda ls a fost terminata fortat de catre semnalul: %d.\n", WTERMSIG(cod_term) );




//printf("SE DORESTE STERGEREA ACESTUI DIRECTOR?");
//char dorinta[10];
//scanf("%s",dorinta);
//if(strcmp(dorinta,"DA")==0||strcmp(dorinta,"Da")==0||strcmp(dorinta,"da")==0)
	// Pasul #3: invocarea celei de-a doua comenzi: rm -r -i nume_dir

	//  : de implementat un switch similar ca la pasul #2, pentru invocarea comenzii rm

switch( fork() )
	{
		case -1:	perror("Eroare la primul fork");  exit(1);

		case  0:	execlp("rm","rm","-r","-i",nume_dir,NULL);
				perror("Eroare la primul exec");  exit(2);

		default:	wait(&cod_term);
				if( WIFEXITED(cod_term) )
					printf("DONE!----Codul de terminare: %d.\n", WEXITSTATUS(cod_term) );
				else
					printf("Comanda rm a fost terminata fortat de catre semnalul: %d.\n", WTERMSIG(cod_term) );
	}







}


}

