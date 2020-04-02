/*[MyWc] Să se scrie un program C care să implementeze comanda wc, inclusiv cu opțiunile sale -c, -w, -l și -L.
(Atenție: se cere să se scrie cod C care realizează aceeași funcționalitate ca și comanda wc, deci nu în sensul ca să apelați direct comanda wc dintr-un program C folosind funcțiile system() sau exec().)

Cerință (valabilă și pentru toate celelalte exerciții de mai jos, acestea fiind asemănătoare cu acest exercițiu):
implementarea comenzii cmd, inclusiv a unei opțiuni -o, presupune următoarele lucruri:
- implementarea comportamentului implicit al comenzii cmd:   cmd nume_fisier
- și implementarea comportamentului modificat pe baza opțiunii precizate în enunț:   cmd -o nume_fisier */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main( int argc, char* argv[])
{
    int no_bytes, no_words, no_lines;  // nr. de octeti, nr. de cuvinte, nr. de linii de text, lungimea maxima a unei linii de text
    int last;      // starea (ce este ultimul caracter/zona citita pana la pozitia curenta): 0 - la inceput de fisier , 1 - un cuvant , 2 - zona de spatii albe
    int fc,fw,fn;  // flaguri pentru optiunile prezente in linia de comanda, respectiv -c ,-w, -n, -L
    int i,ch;
    char *nume_fisier = NULL;
    FILE *fd;

    no_bytes = no_words = no_lines = 0;
    last = 0;
    fc = fw = fn = 0;

    // parsarea liniei de comanda pentru a gasi numele fisierului si optiunile
    for(i = 1; i < argc; i++)
    {
        if(0 == strcmp(argv[i],"-c")) fc = 1;
        else if(0 == strcmp(argv[i],"-w")) fw = 1;
            else if(0 == strcmp(argv[i],"-n")) fn = 1;
                else if(! nume_fisier) nume_fisier = argv[i];
                    else { printf("Programul accepta (deocamdata) un singur nume de fisier, nu mai multe!\n"); exit(1); }
    }
    if( (argc == 1) || (nume_fisier == NULL) )
    {
        fprintf(stderr,"Ati uitat sa specificati numele fisierului de prelucrat!\n");
        exit(2);
    }

    if (0 == fc+fw+fn)
        fc = fw = fn = 1; // daca nici o optiune nu-i prezenta in linia de comanda, le consideram pe toate cele trei, similar ca la comanda wc

    // deschidem fisierul de prelucrat
    if(NULL == (fd = fopen(nume_fisier,"r")))
    {
        perror("Eroare la deschiderea fisierului de prelucrat!\n");
        exit(1);
    }

    // parcurgem fisierul si calculam toate cele trei numere
    while(1)
    {
        ch = fgetc(fd);
        if( feof(fd) ) break;

        no_bytes++;

        if( isspace(ch) )
        {
            if(last == 1)  { no_words++; }
            last = 2;
        }
        else
            last = 1;

        if (ch == '\n') { no_lines++; }
    }

    /* la sfarsit de fisier, daca avem starea 1, trebuie incrementat numarul de cuvinte */
    if (last == 1) { no_words++; }

    /* tot la sfarsit de fisier, daca ultimul caracter nu fusese '\n', trebuie incrementat numarul de linii */
    //if (ch != '\n') { no_lines++; } // nu mai facem asta, caci nici comanda wc nu o face!


    // urmeaza afisarea doar a statisticilor corespunzatoare optiunilor specificate
    printf("Fisierul %s contine: ", nume_fisier);
    if( fc ) printf("%d caractere (octeti) , ", no_bytes);
    if( fw ) printf("%d cuvinte , ", no_words);
    if( fn ) printf("%d linii de text.", no_lines);
    printf("\n");

    return 0;
}

