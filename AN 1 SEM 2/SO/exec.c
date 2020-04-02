
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
        //execlp("cat","cat","-n","-E","/etc/passwd",NULL);
        //execlp("ls","ls","-l","/etc/passwd",NULL);
        //execlp("ls","ls","-l",nume_dir,NULL);
        //execlp("rm","rm","-r","-i",nume_dir,NULL);
//execl("/usr/bin/cut","cut","-d:","-f1,3","/etc/passwd",NULL);
 
		perror("Eroare la exec");
		exit(10); 
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


