/*[MyCritSec #1]
Implementați problema secțiunii critice de la curs în scenariul următor:
Se dă drept resursa partajată de procese un fișier binar, denumit "peco.bin", ce conține un număr real. Acest număr reprezintă volumul curent de combustibil din rezervorul unei stații PECO. Asupra acestei resurse se vor face operațiuni de alimentare a autovehiculelor (i.e. extragere din rezervor) și respectiv de re-alimentare a rezervei stației PECO (i.e. depunere în rezervor) a unei cantități de combustibil.
Scrieți un program C care să efectueze diverse operațiuni de depuneri și extrageri la intervale variate de timp, cantitățile depuse și respectiv extrase fiind specificate ca parametri în linia de comandă, spre exemplu prin apelul:
UNIX> ./prg_sc1 25500 -10,9 -35,42 3500 -20,45 ...
se va efectua mai întâi o depunere a cantității de 25500 litri, apoi o extragere a cantității de 10,9 litri, ș.a.m.d.
Programul va accesa fișierul în manieră cooperantă folosind lacăte în scriere pe durata de efectuare a fiecărei operații de depunere sau extragere, astfel încât să se permită execuția simultană a două sau mai multor instanțe ale programului, fără să apară efecte nedorite datorită fenomenelor de data race.
Operațiunile de depuneri și extrageri vor fi implementate astfel încât să afișeze pe ecran mesaje explicative despre ceea ce se execută, mesajele fiind însoțite de PID-ul procesului ce execută respectiva operațiune. */



#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void initializare_fisier_date();
/*
*  Pentru început, avem nevoie de o rutină ajutătoare, care să creeze conținutul inițial al fișierului de date partajat
*  Se va crea fișierul și va conține un număr real, stocat în format binar, reprezentând cantitatea inițială de combustibil din rezervor.
*  Vom apela programul cu opțiunea -i pentru a invoca această rutină de inițializare.
*/

void afisare_fisier_date();
/*
*  Similar, la finalul tuturor actualizărilor, vom avea nevoie de o altă rutină ajutătoare, care să ne afișeze conținutul final din rezervor.
*  Vom apela programul cu opțiunea -o pentru a invoca această rutină de afișare finală.
*/

void secventa_de_actualizari(int n, char* nr[]);
/*
*  Rutina principală a programului, care va executa secvența de actualizări primită în linia de comandă, în manieră concurentă, conform celor specificate în enunțul problemei.
*  Observație: rutina principală nu este implementată în întregime mai jos, ci vă rămâne dvs. sarcina să completați "bucățile" de cod ce lipsesc (indicate prin comentarii TODO).
*/

int main(int argc, char* argv[])
{
	if(argc == 1)
	{
		fprintf(stderr, "Eroare: programul trebuie apelat cu optiunile: -i, -o, sau cu o secventa de numere reale...\n");  return 1;
	}

	/* cazul de intializare a fisierului de date */
	if( strcmp(argv[1],"-i") == 0 )
	{
		initializare_fisier_date();  return 0;
	}

	/* cazul de afisare a fisierului de date */
	if( strcmp(argv[1],"-o") == 0 )
	{
		afisare_fisier_date();  return 0;
	}

	/* cazul general: actualizarea fisierului de date */
	secventa_de_actualizari(argc, argv);
	return 0;
}


void initializare_fisier_date()
{/* Functionalitate: intializarea fisierului de date */

	int fd;
	float val;

	printf("Dati cantitatea initiala de combustibil (i.e., un numar real pozitiv): ");
	while( (1 != scanf("%f", &val)) && (val < 0) ) { fprintf(stderr,"Eroare: nu ati introdus un numar real pozitiv! Incercati din nou..."); }

	if(-1 == (fd = open("peco.bin", O_WRONLY | O_CREAT | O_TRUNC, 0600) ) )
	{
		perror("Eroare la crearea fisierului de date...");  exit(2);
	}

	if(-1 == write(fd, &val, sizeof(float) ) )
	{
		perror("Eroare la scrierea valorii initiale in fisierul de date...");  exit(3);
	}

	close(fd);
}


void afisare_fisier_date()
{/* Functionalitate: afisarea fisierului de date */

	int fd;
	float val;

	if ( -1 == (fd = open("peco.bin", O_RDONLY) ) )
	{
		perror("Eroare la deschiderea pentru afisare a fisierului de date...");  exit(4); 
	}

	if (-1 == read(fd, &val, sizeof(float) ) )
	{
		perror("Eroare la citirea valorii finale din fisierul de date...");  exit(5); 
	}

	close(fd);
	printf("Stocul final de combustibil este: %f litri de combustibil.\n", val);
}


void secventa_de_actualizari(int n, char* nr[])
{ /* Functionalitate: secventa de operatii de actualizare a fisierului de date */

	float valoare, stoc;
	int fd,i;

	if ( -1 == (fd = open("peco.bin", O_RDWR) ) )
	{
		perror("Eroare la deschiderea pentru actualizare a fisierului de date...");  exit(6); 
	}

	srandom( getpid() ); // initializarea generatorului de numere aleatoare

	for(i = 1; i < n; i++) // iteram prin secventa de numere reale specificata in linia de comanda
	{
		if(1 != sscanf(nr[i], "%f", &valoare))  // facem conversie de la string la float
		{
			fprintf(stderr, "[PID: %d] Eroare: ati specificat o valoare invalida drept argument de tip numar real : %s !\n", getpid(), nr[i] );
			exit(7);
		}

		/* Inceput sectiune critica */
		/*
			... // TODO: implementati singuri punerea lacatului in scriere pe fisier
		*/

		if(-1 == lseek(fd, 0, SEEK_SET) ) // (re)pozitionam cursorul la inceputul fisierului
		{
			perror("Eroare la repozitionarea in fisierul de date, pentru citire...");  exit(8); 
		}
		if (-1 == read(fd, &stoc, sizeof(float) ) )
		{
			perror("Eroare la citirea valorii din fisierul de date...");  exit(9); 
		}

		printf("[PID: %d] Se adauga/extrage in/din rezervor cantitatea de %f litri de combustibil.\n", getpid(), valoare );

		stoc = stoc + valoare;
		/*
			... // TODO: implementati singuri tratarea exceptiei: stoc negativ
			... // Varianta 1: refuzarea operatiei de extragere, cu un mesaj informativ in acest sens
			... // Varianta 2: realizarea unei extrageri partiale, i.e. se extrage doar cat e in stoc, nu cat s-a cerut, cu un mesaj informativ in acest sens
			... // Varianta 3: interogarea utilizatorului, ce varianta de rezolvare doreste? , i.e. varianta 1 sau varianta 2 de mai sus?
		*/

		if(-1 == lseek(fd, 0, SEEK_SET) ) // repozitionam cursorul la inceputul fisierului
		{
			perror("Eroare la repozitionarea in fisierul de date, pentru scriere...");  exit(10); 
		}
		if (-1 == write(fd, &stoc, sizeof(float) ) )
		{
			perror("Eroare la scrierea valorii actualizate in fisierul de date..."); exit(11); 
		}
  
		/* Sfarsit sectiune critica */
		/*
			... // TODO: implementati singuri scoaterea lacatului de pe fisier
		*/
      
		sleep( random() % 5 ); // o pauza aleatoare, de maxim 4 secunde, inainte de a continua cu urmatoarea iteratie
	}     

	close(fd);
	printf("[PID: %d] Am terminat de procesat secventa de actualizari.", getpid());
}



/*   *** Ghid de utilizare ***
Pentru execuție, se va crea mai întâi fișierul partajat, i.e. executând comanda:
UNIX> ./prg_sc1 -i

Apoi se vor rula în paralel mai multe instanțe ale programului, fiecare cu propria secvență de actualizări.
Cu alte cuvinte, lansarea simultană în execuție concurentă a mai multor instanțe ale programului se va face astfel:

UNIX> ./prg_sc1 25500 -10,9 -35,42 3500 -20,45 ... &  ./prg_sc1 -10,5 -34,2 2500 -25,56 ... &  ./prg_sc1 ... & ...

Iar la final veți dori să aflați cantitatea rămasă în rezervor după seria de actualizări de mai sus, i.e. rulați comanda:
UNIX> ./prg_sc1 -o
*/

