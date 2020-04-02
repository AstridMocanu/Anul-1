/*['Supervisor-workers pattern' #2: Coordinated distributed sum v1]
Să se scrie un program C care să realizeze următoarele: un proces P0 (cu rol de proces supervisor) citește numere de la tastatură și le trimite, împărțindu-le în mod egal, la trei procese fii P1, P2 și P3 (cu rol de procese workers), acestea calculează sumele parțiale și le trimit înapoi la părintele P0, iar P0 adună cele trei sume parțiale și afișează rezultatul final.
Pentru comunicația între procese se vor folosi fișiere obișnuite - procesul P0 scrie numerele citite în fișierele f1i, f2i și f3i, de unde sunt citite de procesele P1, P2 și respectiv P3, care le adună și scriu sumele parțiale în fișierele f1o, f2o și f3o, de unde sunt citite de procesul P0 și adunate.
Cerință: programul worker să fie apelat prin exec din fiul creat cu fork de procesul supervisor, iar numele fișierului de intrare (f{1,2,3}i) și respectiv a celui de ieșire (f{1,2,3}o) să-i fie trasferate ca argumente în linia de comandă. */

/*
  Program: calculează suma distribuită, coordonată folosind șablonul supervisor/workers 
  
	Iată o posibilă rezolvare: soluția este formată din două programe sursă C, și anume:

	-- supervisor.c : este programul care citește numerele, creează procesele worker și le dă să calculeze sumele parțiale, iar în final calculează suma finală.
					  Notă: programul este scris astfel încât să creeze nu fix 3 procese worker, ci orice număr (citit la execuție) de procese worker!
	
	-- worker.c : este programul care este apelat în fiecare proces fiu creat de supervisor și care calculează o sumă parțială, a numerelor citite din
				  primul fișier primit ca parametru în linia de comandă, pe care o scrie în al doilea fișier primit ca parametru în linia de comandă.
*/

/*
  Programul: supervisor.c
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_NO_WORKERS      10
#define MAX_LENGTH_FILENAME 50

int main()
{
	int nr=0, k, NW=0, i, pid, sum=0;
	char  input_filenames[MAX_NO_WORKERS][MAX_LENGTH_FILENAME];
	char output_filenames[MAX_NO_WORKERS][MAX_LENGTH_FILENAME];
	FILE *fdi[MAX_NO_WORKERS], *fdo;
 
	printf("Dati numarul de procese workers: ");
	while(1 != scanf("%d", &NW) ) { fprintf(stderr, "Trebuie sa introduceti un numar intreg pozitiv! Incercati din nou...\n"); }
	if( (NW < 1) || (NW > MAX_NO_WORKERS) )
	{
		fprintf(stderr,"Date de intrare incorecte.\n");  exit(1);
	}

	for (i = 1; i <= NW; ++i)
	{
		sprintf( input_filenames[i], "f%di.txt", i);
		sprintf(output_filenames[i], "f%do.txt", i);
	}


	// Pasul #1: citirea numerelor de la tastatura si crearea celor NW fisiere de intrare

	for (i = 1; i <= NW; ++i)
	{
		if( NULL == (fdi[i-1] = fopen(input_filenames[i], "w")) )
		{
			fprintf(stderr,"Supervisor : eroare la deschiderea fisierului de intrare %s.\n", input_filenames[i]);
			perror("Cauza erorii: ");
			exit(2);
		}
	}

	printf("Introduceti secventa de numere (0 pentru terminare) :\n");
	k = 0;
	do{
		while(1 != scanf("%d", &nr) ) { fprintf(stderr, "Trebuie sa introduceti un numar intreg nenul! Incercati din nou...\n"); }
		if(nr != 0)
			fprintf(fdi[k],"%d ",nr);
		k = (k+1) % NW;
	} while(nr != 0);
 
	for (i = 1; i <= NW; ++i)
	{
		fclose(fdi[i-1]);
	}
    

	// Pasul #2: crearea celor NW procese worker care vor executa programul din sursa worker.c, cu parametri diferiti

	for (i = 1; i <= NW; ++i)
	{
		if(-1 == (pid = fork()) )
		{
			fprintf(stderr,"Supervisor : eroare la apelul fork() pentru procesul worker al %d-lea.\n", i);
			perror("Cauza erorii: ");
			exit(3);
		}
		
		if (pid == 0)
		{
			// in fiu apelez cu exec programul worker.exe

			execl("worker.exe", "worker", input_filenames[i], output_filenames[i], NULL);

			fprintf(stderr,"Supervisor : eroare la apelul exec() pentru procesul worker al %d-lea.\n", i);
			perror("Cauza erorii: ");
			exit(4);
		}
		///else
		///    wait (NULL);   Nota: ar fi gresit sa astept aici terminarea fiului, caci ar insemna ca workerii se vor executa secvential, nu in paralel!
	}

	// IMPORTANT: abia dupa ce am startat toate procesele worker, le astept sa se termine de executat
	for (i = 1; i <= NW; ++i)
	{
		wait(NULL);
	}


	// Pasul #3: citesc cele NW sume partiale din fisierele de iesire, ...

	for (i = 1; i <= NW; ++i)
	{
		if (NULL == (fdo = fopen(output_filenames[i], "r")) )
		{
			fprintf(stderr,"Supervisor : eroare la deschiderea fisierului de iesire %s.\n", output_filenames[i]);
			perror("Cauza erorii: ");
			exit(5);
		}
		
		if (1 != fscanf(fdo, "%d", &nr) )
		{
			fprintf(stderr,"Supervisor : eroare la citirea celei de a %d-a sume partiale.\n", i);
			exit(6);
		}
		else
			sum += nr;
		
		fclose(fdo);
	}

	// ..., iar apoi afisez suma totala    
	printf("Supervisor: suma secventei introduse este: %d.\n", sum);

	// In plus, optional, putem facem "curatenie": stergem fisierele de intrare si de iesire
	for (i = 1; i <= NW; ++i)
	{
		remove( input_filenames[i]);
		remove(output_filenames[i]);
	}
	return 0;
}

/*
  Programul: worker.c
*/

#include <stdio.h>

int main(int argc, char *argv[])
{
	int nr, sum=0;
	FILE *fd;

	// Pasul #1: citirea numerelor si calculul sumei
	
	if (NULL == (fd = fopen(argv[1], "r")) )
	{
		fprintf(stderr,"Worker : eroare la deschiderea fisierului de intrare %s.\n", argv[1]);
		return 1;
	}
	while(! feof(fd) )
	{
		if(1 == fscanf(fd, "%d", &nr) )
			sum += nr;
	}
	fclose(fd);


	// Pasul #2: scrierea sumei calculate    

	if (NULL == (fd = fopen(argv[2], "w")) )
	{
		fprintf(stderr,"Worker : eroare la deschiderea fisierului de iesire %s.\n", argv[2]);
		return 2;
	}
	fprintf(fd, "%d\n", sum);
	fclose(fd);
    
	return 0;
}

