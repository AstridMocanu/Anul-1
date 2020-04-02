/*[MyCritSec #2 : Parallel sorting]
Să se scrie un program C care să sorteze concurent un fișier, al cărui nume se va da ca parametru în linia de comandă, în felul descris în continuare.
Fișierul va conține o secvență de numere întregi, neordonată. Programul va ordona crescător numerele din fișier folosind algoritmul Bubble-Sort clasic: se iterează în buclă parcurgeri ale secvenței de numere de la începutul secvenței spre sfârșitul ei, efectuându-se inversiuni ale perechilor ce nu sunt în ordinea 'mai mic decât'. Algoritmul se oprește când la ultima parcurgere nu s-a efectuat nici o inversiune.
La execuție se vor lansa în paralel două (sau mai multe) instanțe ale programului, care vor opera deci simultan asupra fișierului. Ca urmare, programul va trebui să acceseze fișierul în manieră cooperantă, folosind lacăte în scriere pe durata de efectuare a fiecărei inversiuni în fișier pentru a nu apare efecte nedorite datorită fenomenului de race conditions. Lacătele se vor pune numai pe porțiunea strict necesară și numai pe durata minimă necesară (asemănător ca la versiunea 4 a programului demonstrativ access discutat la curs, la lecția practică despre lacăte pe fișiere). */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

void bubble_sort(int filedescr);

int main(int argc,char *argv[])
{
	int fd;
	if(argc < 2)
	{
		printf("Usage: %s datafile\n", argv[0]);  exit(1);
	}

	if(-1 == (fd = open(argv[1],O_RDWR)))
	{
		perror("Eroare la deschiderea fisierului de date");  exit(2);
	}
    
	bubble_sort(fd);

	close(fd);

	fprintf(stderr, "Notificare: [PID: %d] Am terminat (contributia mea la) sortarea fisierului %s !\n", getpid(), argv[1]);
	return 0;
}

void bubble_sort(int filedescr)
{
	int rcod1,rcod2;
	int numar1,numar2;

	int modificare = 1;

	/* bucla in care facem parcurgeri repetate */
	while(modificare)
	{
		modificare = 0; /* va fi setat daca se face macar o inversiune la parcurgerea curenta */

		/* bucla pentru o singura parcurgere a fisierului */
		while(1)
		{
			rcod1 = read(filedescr, &numar1, sizeof(int));
			if(rcod1 == 0) break; /* am ajuns la EOF */
			if(rcod1 ==-1)
			{
				perror("Eroare la citirea primului numar dintr-o pereche");  exit(3);
			}	

			rcod2 = read(filedescr, &numar2, sizeof(int));
			if(rcod2 == 0) break; /* am ajuns la EOF */
			if(rcod2 ==-1)
			{
				perror("Eroare la citirea celui de-al doilea numar dintr-o pereche");  exit(4);
			}	

			/* daca este inversiune, le schimbam in fisier */
			if(numar1 > numar2)
			{
				modificare = 1;
			
				/* ne intoarcem inapoi cu 2 intregi pentru a face rescrierea */
				if(-1 == lseek(filedescr, -2*sizeof(int), SEEK_CUR))
				{
					perror("Eroare (1) la repozitionarea inapoi in fisier");  exit(5);
				}
	
				if(-1 == write(filedescr, &numar2, sizeof(int)))
				{
					perror("Eroare la rescrierea primului numar dintr-o pereche");  exit(6);
				}
			
				if(-1 == write(filedescr, &numar1, sizeof(int)))
				{
					perror("Eroare la rescrierea celui de-al doilea numar dintr-o pereche");  exit(7);
				}
			}
		
			/* pregatim urmatoarea iteratie: primul numar din noua pereche este ce-al doilea numar din perechea precedenta */
			if(-1 == lseek(filedescr, -sizeof(int), SEEK_CUR))
			{
				perror("Eroare (2) la repozitionarea inapoi in fisier");  exit(8);
			}
		}/* sfarsitul buclei pentru o singura parcurgere a fisierului */

		/* pregatim urmatoarea parcurgere: ne repozitionam la inceputul fisierului */
		if(-1 == lseek(filedescr, 0L, SEEK_SET))
		{
			perror("Eroare (3) la repozitionarea inapoi in fisier");  exit(9);
		}

	}/* sfarsitul buclei de parcurgeri repetate */
	
}

/*   *** Ghid de utilizare ***
Pentru execuție, se va crea mai întâi fișierul partajat, i.e. executând comanda:
UNIX> ./bin-write.exe
și introducând o secvență de numere intregi, oricât de lungă și neordonată.
Puteți vizualiza conținutul fișierului fis.dat creat în urma acestei comenzi, cu orice editor hexa, e.g. cu comanda mcview fis.dat și apăsați F4,
și încercați să "depistați" secvențele little-endian corespunzătoare reprezentării binare pentru numerele întregi pe care le-ați introdus.

Apoi puteți încerca să executați în paralel mai multe instanțe ale programului, fiecare cu același fișier de date, astfel:
UNIX> ./sortare_fara-lacate.exe fis.dat & ./sortare_fara-lacate.exe fis.dat & ./sortare_fara-lacate.exe fis.dat & ...

Iar la final veți dori să aflați care este secvența obținută în urma sortării, pentru aceasta rulați comanda:
UNIX> ./bin-read.exe

Ținând cont de cele spuse mai sus, este foarte posibil ca secvența pe care-o veți obține să nu fie cea corectă!
Exemple de posibile erori: unele numere din secvența inițială s-ar putea să nu mai apară deloc în secvența rezultat,
iar alte numere s-ar putea să apară în secvența rezultat de mai multe ori decât apar în secvența de intrare.
*/

