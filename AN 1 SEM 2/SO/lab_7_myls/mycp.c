/*[MyCp] Să se scrie un program C care să implementeze comanda cp, inclusiv cu opțiunile sale -i și -u. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int copie_opts_i_u(char *nume_sursa, char *nume_dest, int flag_opt_i, int flag_opt_u);

int main(int argc, char *argv[])
{
    int i, flag_opt_i=0, flag_opt_u=0;
    char *nume_sursa=NULL;
    char *nume_dest =NULL;

    if (argc < 3)
    {
        fprintf(stderr,"Usage: %s [-i] [-u] fisier-sursa fisier-destinatie\n",argv[0]);
        exit(1);
    }

    /* Prelucram argumentele din linia de comanda pentru a identifica prezenta optiunilor -i si/sau -u,
       precum si a numelor fisierelor sursa si destinatie.
       Observatie: vom permite ca optiunile -i si -u sa apara in orice pozitie in linia de comanda.
    */
    for(i = 1; i < argc; i++)
    {
        if( strcmp(argv[i],"-i") == 0 )   {  flag_opt_i = 1;  continue;  }
        if( strcmp(argv[i],"-u") == 0 )   {  flag_opt_u = 1;  continue;  }

        if( nume_sursa == NULL )  {  nume_sursa = argv[i];  continue;  }
        if( nume_dest  == NULL )  {  nume_dest  = argv[i];  continue;  }

        fprintf(stderr, "Eroare: nu ati respectat formatul impus pentru argumentele din linia de comanda.\n");
        exit(2);
    }

    if( nume_sursa == NULL )  { fprintf(stderr, "Eroare: nu ati specificat fisierul sursa.\n"); exit(3); }

    if( nume_dest  == NULL )  { fprintf(stderr, "Eroare: nu ati specificat fisierul destinatie.\n"); exit(4); }


    /* Acum urmeaza apelul unei functii care implementeaza functionalitatea efectiva de copiere ceruta in enunt */

    return copie_opts_i_u(nume_sursa, nume_dest, flag_opt_i, flag_opt_u);
}


int copie_simpla(char *nume_sursa, char *nume_dest);

int copie_opts_i_u(char *nume_sursa, char *nume_dest, int flag_opt_i, int flag_opt_u)
{
    if( access(nume_sursa,R_OK) != 0 )
    {
        fprintf(stderr,"Eroare: fisierul sursa %s nu este accesibil (i.e. nu exista, sau nu am drept de citire, sau ce?)\t",nume_sursa);
        perror("Cauza exacta este");
        return 10;
    }

    int flag_dest_exista = access(nume_dest,F_OK)==0 ? 1 : 0 ;  // the conditional operator in C

    int flag_abort_copiere = 0;

    if( flag_dest_exista )
    {
        /* trebuie sa verificam vechimea destinatiei ? (optiunea -u) */

        if(flag_opt_u)
        {
           struct stat st_sursa, st_dest;

           if(0 != stat(nume_sursa,&st_sursa) )
           {
               fprintf(stderr,"Eroare la stat pentru fisierul sursa %s .\t", nume_sursa);  perror("Cauza este");
               return 11;
           }
           if(0 != stat(nume_dest,&st_dest) )
           {
               fprintf(stderr,"Eroare la stat pentru fisierul destinatie %s .\t", nume_dest);  perror("Cauza este");
               return 12;
           }

           if(st_sursa.st_mtime > st_dest.st_mtime)
           {
               printf("\tSursa %s are 'last modified time': %s", nume_sursa, ctime(&st_sursa.st_mtime) );
               printf("\tDestinatia %s are 'last modified time': %s", nume_dest, ctime(&st_dest.st_mtime));
               printf("Notify message: copierea se va face cu suprascrierea destinatiei (deoarece sursa este mai recenta decat destinatia).\n");
           }
           else
           {
               flag_abort_copiere = 1;
               printf("Sursa %s nu este mai recenta decat destinatia %s. Deci NU suprascriu destinatia cu sursa!\n", nume_sursa, nume_dest);
           }
        }

        /* mai trebuie sa interogam utilizatorul ? (optiunea -i) */

        if( !flag_abort_copiere && flag_opt_i)
        {  /* dam prioritate optiunii -u : daca destinatia este mai recenta, nu mai intrebam utilizatorul */
           char raspuns;

           printf("Fisierul destinatie %s exista deja (dar este mai vechi decat sursa). Vreti sa-l suprascrieti? (D/N)\n",nume_dest);
           do{
               scanf("%c",&raspuns);
               if( (raspuns != 'D') && (raspuns != 'N') )
                   printf("Raspundeti cu D sau N.\n");
               else
                   break;
           }while(1);

           if(raspuns == 'N')
               flag_abort_copiere = 1;
           else
               printf("Notify message: copierea se va face cu suprascrierea destinatiei (cu acceptul utilizatorului).\n");
        }
    }


    if(! flag_abort_copiere)
    {
         return copie_simpla(nume_sursa, nume_dest);
    }
    else
    {
         fprintf(stderr, "Copierea lui %s in %s nu s-a mai efectuat.\n", nume_sursa, nume_dest);
         return 13;
    }
}


int copie_simpla(char *nume_sursa, char *nume_dest)
{
    FILE *f_sursa, *f_dest;
    char ch;

    if( NULL == (f_sursa = fopen(nume_sursa,"r")) )
    {
        fprintf(stderr,"Eroare la deschiderea fisierului sursa %s\n",nume_sursa);  perror("Cauza este");
        return 20;
    }

    if( NULL == (f_dest = fopen(nume_dest,"w")) )
    {
        fprintf(stderr,"Eroare la deschiderea fisierului destinatie %s\n",nume_dest);  perror("Cauza este");
        return 21;
    }

    while(1)    /* Echivalent:  while(! feof(f_sursa)) */
    {
        int cod_r;
        cod_r = fscanf(f_sursa,"%c",&ch);
        if(cod_r == EOF) break;
               /* Notă: apelul feof() intoarce true abia dupa ce un apel fscanf()/fread() "citeste" pozitia End-Of-File,
                        situatie in care variabila noastra ch va ramane cu valoarea veche (de la iteratia precedenta),
                        deci trebuie sa ne asiguram ca nu prelucram de doua ori ultimul caracter din fisier !!!
               */
        if(cod_r != 1)
        {
            fprintf(stderr,"Eroare la citirea din fisierul %s\n",nume_sursa);  perror("Cauza este");
            return 22;
        }

        if( fprintf(f_dest,"%c",ch) != 1 )
        {
            fprintf(stderr,"Eroare la scrierea in fisierul %s\n",nume_dest);  perror("Cauza este");
            return 23;
        }

    }

    fclose(f_sursa);
    fclose(f_dest);

    printf("Am copiat cu succes fisierul sursa %s in destinatia %s\n",nume_sursa,nume_dest);
    return 0;
}

