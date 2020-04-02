#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pwd.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>


int main (int argc, char* argv[])
{
char fis1[500], fis2[500];
char a,b;

if(argc>5)
{
perror("prea multe arg citite");
exit(1);
}
else

if(argc == 1)
{
//strcpy(fis1,argv[1]);
printf("dati numele fis 1:");
scanf("%s", fis1);

printf("dati nume fis 2:");
scanf("%s", fis2);

printf("dati caracterul de inlocuit:");
scanf("%c",&a);
scanf("%c",&a);

printf("dati caracterul cu care inlocuim:");
scanf("%c",&b);
scanf("%c",&b);
}


if(argc == 3)
{
strcpy(fis1, argv[1]);
strcpy(fis2,argv[2]);

printf("dati caracterul de inlocuit:");
scanf("%c",&a);
scanf("%c",&a);

printf("dati caracterul cu care inlocuim:");
scanf("%c",&b);
scanf("%c",&b);
}


if(argc == 4)
{
strcpy(fis1, argv[1]);
strcpy(fis2,argv[2]);
a=argv[3];

printf("dati caracterul cu care inlocuim:");
scanf("%c",&b);
scanf("%c",&b);
}



if(argc == 5)
{
strcpy(fis1, argv[1]);
strcpy(fis2,argv[2]);
a=argv[3];
b=argv[4];
}



int fd;
fd=open(fis1, O_RDONLY);
if(fd == -1)
{
perror("eroare la deschidere fis in");
exit(1);
}

unsigned char ch;
int cod_r;

int f2;
f2=open(fis2,O_WRONLY);
if(f2 == -1)
{
perror("eroare la deschidere fis intrare 2");
exit(2);
}

do{
cod_r=read(fd, &ch, 1);

if(cod_r == -1)
{
perror("eroare la citire");
exit(3);
}
if(cod_r == 0 )
break;


char ch2;
ch2=ch;
if(ch==a)
{
ch2=b;
}

write(f2,&ch2,1);

}
while(1);

if(close(fd) == -1)
{
perror("eroare la inchidere");
exit(4);
}

if(close(f2) == -1)
{
perror("er inchidere");
exit(5);
}

return 0;
}
