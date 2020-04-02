#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[]){
char nfis[100];

///citire_argumente//////


if(argc==1)
{
printf("dati numele fis de prelucrat:");
scanf("%s",nfis);
}
else strcpy(nfis,argv[1]);

//initializare_vector///

int v[256],i;
for(i=0;i<256;i++)
v[i]=0;

////conectare_fisier////


int conect;

if((conect=open(nfis,O_RDONLY))==-1)
{
perror("eroare la desch fis");
exit(2);
}

///numarare_caractere///

char c;
int cod_r;
do{
if((cod_r=read(conect,&c,1)))
{
perror("eroare la citire");
exit(3);
}
if(cod_r==0) ///EOF
break;

v[c]++;
}
while(1);

///inchiderea_conexiunii/////
if(close(conect)==-1)
{
perror("Eroare ls inchidere");
exit(4);
}

////afisare////
for(i=0;i<256;i++)
if(v[i]!=0)
printf("caracterul %c apare de %d ori\n",i,v[i]);

return 0;}
