/*[ArithmeticMean]
Să se scrie un program C care să realizeze prelucrările descrise în continuare. Într-un fișier text este stocată o secvență de numere întregi. Să se calculeze media aritmetică pentru fiecare grup de numere cuprinse între două numere cu valoarea zero din fișier. Să se scrie valorile calculate pe câte o linie distinctă dintr-un alt fișier text (fișierul rezultat). Începutul și sfârșitul fișierului cu secvența de numere "joacă rolul" unui număr cu valoarea zero. Numele fișierelor sursă și destinație se specifică drept argumente la linia de comandă a programului. */

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>

int main(int argc, char* argv[])
{

if(argc<3)
{
fprintf(stderr,"Usage %s fisier-sursa fisier-destinatie\n",argv[0]);
exit(1);
}

//verif_dreptului_de_citire///
if(-1==access(argv[1],R_OK))///dreptul de citire
{
fprintf(stderr,"eroare la acces");
exit(2);
}



FILE *sursa,*dest;
///conectarea///
if(NULL==(sursa=fopen(argv[1],'r')))
{
fprintf(stderr,"eroare la desch read-only");
exit(3);
}

if(NULL==(dest=fopen(argv[2],'w')))
{
fprintf(stderr,"eroare la desch fis write_only");
exit(4);
}

///obtinere medie////
int sum=0,nr=0,val=0;
while(1)////echiv cu while(!feof(f_sursa))
{
int cod_r;
cod_r=fscanf(sursa,"%d",&val);

if(cod_r==EOF)break;

if(cod_r!=1)
{
fprintf(stderr,"ERoare la citirea din fis",argv[1]);
perror("cauza este:");
exit(5);
}

if(val!=0)
{sum+=val;
nr++;
}
else
{if(nr!=0)
fprintf(dest,"media este:%.2f\n",(float)sum/nr);
sum=0;
nr=0;}
}

////nu se scrie in fisier si pentru ultima val
//asa ca scriem la iesirea din while

if(nr!=0)fprintf(dest,"%.2f\n",(float)sum/nr);


///inchiderea fisierelor///

fclose(sursa);
fclose(dest);

return 0;}
