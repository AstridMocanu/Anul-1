/*["Hi-ho, hi-ho" v1, i.e. 'Ping-pong pattern' #3, using fifos for IPC]
Să se scrie două programe, hi.c și ho.c, care să producă următorul efect atunci când sunt executate simultan (în paralel): primul să scrie pe ecran textul "hi-" în mod repetat, iar al doilea să scrie pe ecran testul "ho, " în mod repetat, și, în plus, trebuie să se sincronizeze procesele, folosind canale fifo, astfel încât pe ecran să apară exact succesiunea: hi-ho, hi-ho, hi-ho, ... și nu alte combinații posibile de interclasare a mesajelor afișate de cele două procese atunci când sunt lansate în execuție simultană. */


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
	int fd_in, fd_out, nr_iteratii=100;
	char ch='0';

	/* creare canale fifo */
	if(-1 == mkfifo("fifo_hi2ho", 0600) )
	{
		if(errno != EEXIST)   // errno=17 for "File already exists"
		{
			perror("Eroare la crearea canalului 'fifo_hi2ho'. Cauza erorii");  exit(1);
		}
	}

	/* deschiderile necesare, doar a capetelor de scriere sau de citire absolut necesare */
	fd_out = open("fifo_hi2ho",O_WRONLY);
	fd_in  = open("fifo_ho2hi",O_RDONLY);

	/* bucla principala, ce afiseaza "hi"-uri pe ecran, conform sablonului de sincronizare specificat
	while(nr_iteratii > 0)
	{
		nr_iteratii--;

		printf("Hi-");  fflush(stdout);

		write(fd_out,&ch,1); // il "notific" pe celalalt ca este randul lui sa afiseze pe ecran

		read(fd_in,&ch,1);  // astept sa "primesc notificare" de la celalalt ca pot continua
	}

	return 0;
}


/*
  Programul #2: ho.c , este programul care afiseaza "ho"-uri pe ecran.
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
	int fd_in, fd_out, nr_iteratii=100;
	char ch='1';

	/* creare canale fifo */
	if(-1 == mkfifo("fifo_hi2ho", 0600) )
	{
		if(errno != EEXIST)   // errno=17 for "File already exists"
		{
			perror("Eroare la crearea canalului 'fifo_hi2ho'. Cauza erorii");  exit(1);
		}
	}
	if(-1 == mkfifo("fifo_ho2hi", 0600) )
	{
		if(errno != EEXIST)   // errno=17 for "File already exists"
		{
			perror("Eroare la crearea canalului 'fifo_ho2hi'. Cauza erorii");  exit(2);
		}
	}

	/* deschiderile necesare, doar a capetelor de scriere sau de citire absolut necesare */
	fd_in  = open("fifo_hi2ho",O_RDONLY);
	fd_out = open("fifo_ho2hi",O_WRONLY);

	/* bucla principala, ce afiseaza "ho"-uri pe ecran, conform sablonului de sincronizare specificat
	while(nr_iteratii > 0)
	{
		nr_iteratii--;

		read(fd_in,&ch,1);  // astept sa "primesc notificare" de la celalalt ca pot continua

		printf("ho, ");  fflush(stdout);

		write(fd_out,&ch,1); // il "notific" pe celalalt ca este randul lui sa afiseze pe ecran
	}

	printf("\nSfarsit hi-ho!\n");
	return 0;
}



*** Ghid de utilizare ***

Cele două programe trebuie lansate simultan în execuție paralelă, i.e. cu o linie de comandă de felul următor:

UNIX> ./hi.exe & ./ho.exe &


*** Observații ***

#1: Prin modul de lansare în execuție paralelă descris mai sus, cele două procese nu mai sunt înrudite (în mod direct),
    restricție ce ne era necesară la problemele similare din laboratorul precedent ce foloseau canale anonime pentru IPC.

#2: Ordinea în care se deschid cele două canale fifo în cele două programe este esențială -- trebuie deschise în aceeași ordine
    în ambele programe, căci altfel ar apare inter-blocaj: fiecare proces ar aștepta la infinit, fiind blocat la primul apel open() din program.

#3: Deoarece în cele două programe am ales, ca ordine de deschidere, să deschidem mai întâi canalul "fifo_hi2ho" și apoi canalul "fifo_ho2hi",
    au fost suficiente doar 1+2=3 apeluri mkfifo() dispuse ca în codul mai sus (și nu 4 apeluri, câte 2 în fiecare program) pentru
    a garanta faptul că, la orice execuție paralelă posibilă a celor două programe (i.e., indiferent de interleaving-ul pe procesor),
    apelurile open() nu vor eșua datorită motivului "fișier inexistent".
    Evident, mai sunt și alte metode de a garanta acest fapt, prin care se pot folosi chiar și numai 2 apeluri mkfifo().
