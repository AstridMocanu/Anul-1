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

if(argc>3)
{
perror("prea multe arg citite");
exit(1);
}


if(argc == 1)
{
//strcpy(fis1,argv[1]);
printf("dati numele fis 1:");
scanf("%s", fis1);

printf("dati nume fis 2:");
scanf("%s", fis2);
}
if(argc == 2)
{
strcpy(fis1, argv[1]);

printf("dati nume fis 2:");
scanf("%s", fis2);

}


if(argc == 3)
{
strcpy(fis1, argv[1]);
strcpy(fis2,argv[2]);

}





FILE* fd;
fd=fopen(fis1, "r");
if(fd == -1)
{
perror("eroare la deschidere fis in");
exit(1);
}

unsigned char ch;
int cod_r;

FILE* f2;
f2=fopen(fis2,"w");
if(f2 == -1)
{
perror("eroare la deschidere fis intrare 2");
exit(2);
}

  fseek(fd,0,SEEK_END);
  int fin = ftell(fd);
  fseek(f2, -2, SEEK_END);

  int poz;

while(ftell(fd)!=0){

fscanf(fd, "%c",&ch);

if(ch=='\n')
{
poz=ftell(fd);
while (1)
    {
    if (fscanf(fd, "%c",&ch) == 0)
        break;
    if (ch == '\n' || ftell(fd) == fin)
        break;
        fprintf(f2,"%c", ch);
    }
      fprintf(f2, "\n");
      fseek(fd,poz,SEEK_SET);
    }
      fseek(fd, -2, SEEK_CUR);
  }

while (1)
      {
        if (fscanf(fd,"%c",&ch) == 0)
          break;
          if (ch == '\n')
          break;
        fprintf(f2,"%c",ch);
      }




if(fclose(fd) == -1)
{
perror("eroare la inchidere");
exit(4);
}

if(fclose(f2) == -1)
{
perror("er inchidere");
exit(5);
}

return 0;
}

